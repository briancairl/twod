/**
 * @copyright 2018 TwoD
 * @author Brian Cairl
 *
 * @file crtp.h
 */

#ifndef TWOD_CRTP_H
#define TWOD_CRTP_H

/**
 * @brief Macro used to implement CRTP base
 * @param DerivedT  CRTP-derived type
 */
#define IMPLEMENT_CRTP_BASE_CLASS(Derived)\
inline Derived* derived()\
{\
  return static_cast<Derived*>(this);\
}\
inline const Derived* derived() const\
{\
  return static_cast<const Derived*>(this);\
}

#endif // TWOD_CRTP_H