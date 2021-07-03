#pragma once
#include <cstdint>
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iomanip>

namespace RRND {
  class Basic {
  public:
    static bool     set_seed(uint32_t t_seed);
    static uint32_t get_seed();
    template<typename T=uint32_t>
    static T        random(T t_min, T t_max);
  protected:
  private:
    class Ratio {
    public:
    protected:
    private:
      class CRatioFeature;
      static CRatioFeature* get_ratio_randomizer();
      static void           add(uint32_t t_id, uint32_t t_weight, CRatioFeature* t_ratio_random_ptr);
      static bool           remove(uint32_t t_id, CRatioFeature* t_ratio_random_ptr);
      static uint32_t       random(CRatioFeature* t_ratio_random_ptr);
      static void           dump(CRatioFeature* t_ratio_random_ptr);

      template<class T>
      friend class Core;
    };
    template<class T>
    friend class Core;
  };

  template<class CLASS_C = uint32_t>
  class Core {
  public:
    Core();

    void                                add(const CLASS_C& t_obj, uint32_t t_weight);
    bool                                remove(size_t t_position);
    const CLASS_C&                      random();
    uint32_t                            size() const;
    std::pair<CLASS_C&, uint32_t>       at(size_t t_position);
    const std::pair<const CLASS_C&, uint32_t>& at(size_t t_position) const;
    const void                          dump() const;
    
  protected:
  private:
    uint32_t m_idx;
    std::map<uint32_t, std::pair<CLASS_C, uint32_t>> m_objMap;
    Basic::Ratio::CRatioFeature* m_randomPtr;
  };
}


template <class CLASS_C>
RRND::Core<CLASS_C>::Core() :m_idx(0), m_randomPtr(Basic::Ratio::get_ratio_randomizer()) {
}

template <class CLASS_C>
void RRND::Core<CLASS_C>::add(const CLASS_C& t_obj, uint32_t t_weight) {
  m_objMap.insert({ m_idx, {t_obj, t_weight } });
  Basic::Ratio::add(m_idx, t_weight, m_randomPtr);
  ++m_idx;
}

template <class CLASS_C>
bool RRND::Core<CLASS_C>::remove(size_t t_position) {
  m_objMap.erase(t_position);

  return Basic::Ratio::remove(t_position, m_randomPtr);
}

template <class CLASS_C>
const CLASS_C& RRND::Core<CLASS_C>::random() {
  return m_objMap.at(Basic::Ratio::random(m_randomPtr)).first;
}

template <class CLASS_C>
uint32_t RRND::Core<CLASS_C>::size() const {
  return (uint32_t)m_objMap.size();
}

template <class CLASS_C>
std::pair<CLASS_C&, uint32_t> RRND::Core<CLASS_C>::at(size_t t_position) {
  return { m_objMap.at(t_position).first, m_objMap.at(t_position).second };
}

template <class CLASS_C>
const std::pair<const CLASS_C&, uint32_t>& RRND::Core<CLASS_C>::at(size_t t_position) const {
  return { m_objMap.at(t_position).first, m_objMap.at(t_position).second };
}

template <class CLASS_C>
const void RRND::Core<CLASS_C>::dump() const {
  Basic::Ratio::dump(m_randomPtr);
}
