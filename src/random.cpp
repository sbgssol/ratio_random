#include "random.h"
#include <iostream>
#include <random>
#include <exception>
#include <cstdint>
#include <typeinfo>
#include <map>
#include <iomanip>
#include <typeinfo>

namespace RRND {
  /*============= Class CBasicFeature declaration =============*/
  class CBasicFeature {
  public:
    static CBasicFeature* getInstance();
    
    bool                  setSeed(uint32_t t_seed);
    
    uint32_t              getSeed() const;

    template<typename T = uint32_t>
    T random(T t_min, T t_max);
    
  protected:
  private:
    CBasicFeature() :m_seed(0), m_engine{ std::random_device{}() } {}
    ~CBasicFeature() = default;

    uint32_t              m_seed;
    std::mt19937          m_engine;
    static CBasicFeature* m_self;
  };

  /*============= Class CBasicFeature definition =============*/
  CBasicFeature* CBasicFeature::getInstance() {
    if (m_self == nullptr) {
      m_self = new CBasicFeature();
    }

    return m_self;
  }

  bool CBasicFeature::setSeed(uint32_t t_seed) {
    if (m_seed) {
      return false;
    }

    m_seed = t_seed;
    m_engine.seed(t_seed);

    return true;
  }

  uint32_t CBasicFeature::getSeed() const {
    return m_seed;
  }

  template <typename T>
  T CBasicFeature::random(T t_min, T t_max) {
    if (t_min == t_max) {
      return t_min;
    }

    if (t_min > t_max) {
      std::swap(t_min, t_max);
    }

    if (m_seed == 0) {
      throw std::runtime_error{ "ERROR: Random seed was not set!" };
    }

    return (std::uniform_int_distribution<T>(t_min, t_max)(m_engine));
  }

  /*============================/END/=============================*/

  CBasicFeature* CBasicFeature::m_self{ nullptr };

  /*============= Class CRatioFeature declaration =============*/
  /**
   * \brief This class is hidden from user, contains all data and performs random
   */
  class Basic::Ratio::CRatioFeature {
    using ID = uint32_t;
    using WEIGHT = uint32_t;
  public:
    CRatioFeature() = default;

    void add(ID t_id, WEIGHT t_weight);
    bool remove(ID t_id);
    ID   random() const;
    void dump() const;
  protected:
  private:
    std::map<ID, WEIGHT> m_objList; // Contains <ID, WEIGHT>
  };

  /*============= Class CRatioFeature definition =============*/
  void Basic::Ratio::CRatioFeature::add(ID t_id, WEIGHT t_weight) {
    if(t_weight == 0) {
      return;
    }
    m_objList.insert({ t_id, t_weight });
  }

  bool Basic::Ratio::CRatioFeature::remove(ID t_id) {
    m_objList.at(t_id) = 0;
    return true;//m_objList.erase(t_id);
  }

  // BUG: Random in range of inserted idx, meanwhile, there are idx(s) that maybe removed

  Basic::Ratio::CRatioFeature::ID Basic::Ratio::CRatioFeature::random() const {
    if(m_objList.empty()) {
      throw std::runtime_error{ "ERROR: Object list was empty to random!" };
    }

    uint32_t total_weight = 0;
    for(const auto& p : m_objList) {
      total_weight += p.second;
    }

    const uint32_t minimum_random_weight = 1;
    const uint32_t random_weight = CBasicFeature::getInstance()->random(minimum_random_weight, total_weight);

    uint32_t current_weight = 0;
    ID result = (*m_objList.begin()).first;
    for(const auto& p : m_objList) {
      if(current_weight + p.second >= random_weight) {
        result = p.first;
        break;
      }

      current_weight += p.second;
    }

    return result;
  }

  void Basic::Ratio::CRatioFeature::dump() const {
    for(const auto& p : m_objList) {
      std::cout << "ID: " << p.first << ", W: " << p.second << '\n';
    }
    std::cout << "====================" << '\n';
  }

  /*============================/END/=============================*/
    /*============= Header functions definition =============*/
  bool Basic::set_seed(uint32_t t_seed) {
    return CBasicFeature::getInstance()->setSeed(t_seed);
  }

  uint32_t Basic::get_seed() {
    return CBasicFeature::getInstance()->getSeed();
  }

  Basic::Ratio::CRatioFeature* Basic::Ratio::get_ratio_randomizer() {
    return (new CRatioFeature());
  }

  void Basic::Ratio::add(uint32_t t_id, uint32_t t_weight, CRatioFeature* t_ratio_random_ptr) {
    t_ratio_random_ptr->add(t_id, t_weight);
  }

  bool Basic::Ratio::remove(uint32_t t_id, CRatioFeature* t_ratio_random_ptr) {
    return t_ratio_random_ptr->remove(t_id);
  }

  uint32_t Basic::Ratio::random(CRatioFeature* t_ratio_random_ptr) {
    return t_ratio_random_ptr->random();
  }

  void Basic::Ratio::dump(CRatioFeature* t_ratio_random_ptr) {
    t_ratio_random_ptr->dump();
  }

  template <typename T>
  T Basic::random(T t_min, T t_max) {
    return CBasicFeature::getInstance()->random(t_min, t_max);
  }



  // Specialization -> random
  template int32_t    RRND::Basic::random<int32_t>(int32_t, int32_t);
  template int64_t    RRND::Basic::random<int64_t>(int64_t, int64_t);
  template uint32_t   RRND::Basic::random<uint32_t>(uint32_t, uint32_t);
  template uint64_t   RRND::Basic::random<uint64_t>(uint64_t, uint64_t);

}
