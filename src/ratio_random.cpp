#include "ratio_random.h"

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <unordered_map>

namespace RRND {
  /*============= Class CBasicFeature declaration =============*/
  class CBasicFeature {
  public:
    static CBasicFeature* getInstance();    
    bool                  setSeed(uint32_t t_seed);    
    uint32_t              getSeed() const;
    template<typename T = uint32_t>
    T                     random(T t_min, T t_max);
  private:
    uint32_t              m_seed   { 0 };
    std::mt19937          m_engine { std::random_device{}() };
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
    ID   random(std::vector<std::pair<ID, WEIGHT>> const &);
    void dump() const;
    void change_weight(ID t_id, WEIGHT t_new_weight);
  private:
    std::vector<std::pair<ID, WEIGHT>> m_obj;
  };

  /*============= Class CRatioFeature definition =============*/
  void Basic::Ratio::CRatioFeature::add(ID t_id, WEIGHT t_weight) {
    //m_objList.insert({ t_id, t_weight });
    m_obj.emplace_back(std::make_pair(t_id, t_weight));
  }

  bool Basic::Ratio::CRatioFeature::remove(ID t_id) {
    if(t_id >= m_obj.size()) {
      return false;
    }
    m_obj.erase(m_obj.begin() + t_id);
    return true;
  }

    Basic::Ratio::CRatioFeature::ID Basic::Ratio::CRatioFeature::random() const {
    if(m_obj.empty()) {
      throw std::runtime_error{ "ERROR: There is no object to random!" };
    }

    uint32_t const total_weight = std::accumulate(m_obj.begin(), m_obj.end(), 0, [](uint32_t s, std::pair<ID, WEIGHT> const& p) {
      return s + p.second;
    });
    uint32_t const minimum_random_weight = 1;
    uint32_t const random_weight = CBasicFeature::getInstance()->random(minimum_random_weight, total_weight);

    ID idx = 0;
    for(uint32_t current_weight = 0; idx < m_obj.size() - 1; ++idx) {
      current_weight += m_obj[idx].second;
      if(current_weight >= random_weight) {
        break;
      }
    }

    return idx;
  }

  Basic::Ratio::CRatioFeature::ID Basic::Ratio::CRatioFeature::random(std::vector<std::pair<ID, WEIGHT>> const& t_lst) {
    std::vector<std::pair<ID, WEIGHT>> temporary;
    std::copy(m_obj.begin(), m_obj.end(), std::back_inserter(temporary));
    m_obj.clear();
    std::copy(t_lst.begin(), t_lst.end(), std::back_inserter(m_obj));

    ID const id = this->random();

    m_obj.clear();
    std::copy(temporary.begin(), temporary.end(), std::back_inserter(m_obj));
    return id;
  }

  void Basic::Ratio::CRatioFeature::dump() const {
    for(auto const& p : m_obj) {
      std::cout << "IDX: " << p.first << ", W: " << p.second << '\n';
    }
    std::cout << "====================" << '\n';
  }

  void Basic::Ratio::CRatioFeature::change_weight(ID t_id, WEIGHT t_new_weight) {
    m_obj.at(t_id).second = t_new_weight;
  }

  /*============================/END/=============================*/
    /*============= Header functions definition =============*/
  bool Basic::set_seed(uint32_t t_seed) {
    return CBasicFeature::getInstance()->setSeed(t_seed);
  }

  uint32_t Basic::get_seed() {
    return CBasicFeature::getInstance()->getSeed();
  }

  Basic::Ratio::CRatioFeature* Basic::Ratio::get_ratio_modified_engine() {
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

  void Basic::Ratio::change_weight(uint32_t t_id, uint32_t t_weight, CRatioFeature* t_ptr) {
    t_ptr->change_weight(t_id, t_weight);
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
