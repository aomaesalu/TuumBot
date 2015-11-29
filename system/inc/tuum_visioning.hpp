/** @file tuum_visioning.hpp
 *  Visioning interface declaration.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 26 November 2015
 */

#ifndef RTX_VISIONING_H
#define RTX_VISIONING_H

#include <vector>

#include <boost/thread/mutex.hpp>
#include <boost/coroutine/coroutine.hpp>

#include "rtxmath.hpp"

#include "rtxhal.hpp"
#include "rtxent.hpp"
#include "rtxvision.h"

using namespace boost;
using namespace vis;

namespace rtx { namespace Visioning {

  //TODO: replace this function's usage with an analoguous Entity method
  inline double stateProbability(Transform* t1, Transform* t2) {
    double px = gaussian_probability(t1->getX(), 30, t2->getX());
    double py = gaussian_probability(t1->getY(), 30, t2->getY());
    return (px + py) / 2;
  }

  typedef std::vector<Feature*> FeatureSet;
  typedef std::vector<Ball*> BallSet;
  typedef std::vector<Robot*> RobotSet;

  // Entity Detection State
  template<class T>
  struct EDS {
    int mn_h = -5; // Entity removal health condition
    int mx_h = 2;  // Entity detection health condition

    std::vector<T*> objs;
    std::vector<T*> tmp_objs;
    mutex mLock;

    std::vector<T*>* getEntities() {
      mutex::scoped_lock scoped_lock(mLock);
      return &objs;
    }

    std::vector<T*>* getTmpEntities() {
      mutex::scoped_lock scoped_lock(mLock);
      return &tmp_objs;
    }

    auto getAllEntities() {
      EDS* that = this;

      typename boost::coroutines::asymmetric_coroutine<T*>::pull_type routine(
        [that](typename boost::coroutines::asymmetric_coroutine<T*>::push_type& sink){
          for(auto& o : that->objs) sink(o);
          for(auto& o : that->tmp_objs) sink(o);
      });

      return routine;
    }

    void processProbableEntity(T* obj) {
      // Calculate entity similarity probabilities
      T* probable_entity;
      double p = 0.0, _p;

      // Calculate balls similarity probabilities
      for(auto& o : getAllEntities()) {
        // TODO: entity object should implement this probability method
        _p = stateProbability(o->getTransform(), obj->getTransform());

        if(_p > p) {
          p = _p;
          probable_entity = o;
        }
      }

      // Create or update balls
      if(p < 0.009) {
        tmp_objs.push_back(new T(*obj));
      } else {
        probable_entity->update(*obj->getTransform());
      }
    }

    int size() { return objs.size(); }
    int probableSize() { return tmp_objs.size(); }

    void update() {
      {
        for(auto& b : objs) {
          b->update();
        }

        for(auto& b : tmp_objs) {
          b->update();
        }
      }

      int health = mn_h;

      //FIXME: Memory leaks?
      tmp_objs.erase(std::remove_if(tmp_objs.begin(), tmp_objs.end(),
        [health](T*& obj_ptr) {
          return obj_ptr->getHealth() < health;
        }), tmp_objs.end());

      {
        auto it = tmp_objs.begin();
        while(it != tmp_objs.end()) {
          if((*it)->getHealth() > mx_h) {
            objs.push_back(*it);
            it = tmp_objs.erase(it);
          } else it++;
        }
      }

      {
        auto it = objs.begin();
        while(it != objs.end()) {
          if((*it)->getHealth() < mx_h) {
            tmp_objs.push_back(*it);
            it = objs.erase(it);
          } else it++;
        }
      }
    }

  };

  extern std::string filter;

  extern FeatureSet features;

  extern EDS<Ball> ballDetect;
  extern BallSet balls; //TODO: refactor code to utilize 'ballDetect'
  extern BallSet ballsBuffer; // Unused

  extern Goal *blueGoal;
  extern Goal *blueGoalBuffer;
  extern Goal *yellowGoal;
  extern Goal *yellowGoalBuffer;

  extern EDS<Robot> robotDetect;
  extern RobotSet robots;
  extern RobotSet robotsBuffer; // Unused

  extern bool editingBalls;
  extern bool editingGoals;
  extern bool editingRobots;

  void setup();
  void process();
  void processCheckerboard();

  void readFilterFromFile(const std::string&);

  void featureDetection(const Frame&);
  void ballDetection(const Frame&);
  void goalDetection(const Frame&);
  void robotDetection(const Frame&);

}}

#endif // RTX_VISIONING_H
