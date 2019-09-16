#include "integrand.h"

/**
* This function evaluates the definite integrals presented in the model paper
* It solves for the radial normall stress, as well as internal pressurization
* needed for a desired actuation of the IAB material
*/

template<typename value_type, typename function_type>
inline value_type integrator(const value_type a,
                             const value_type b,
                             const value_type atol,
                             const value_type reltol,
                             function_type func)
{
  unsigned n = 1U;

  value_type h = (b - a);
  value_type I = (func(a) + func(b)) * (h / 2);

  for(unsigned k = 0U; k < 8U; k++)
  {
     h /= 2;

     value_type sum(0);
     for(unsigned j = 1U; j <= n; j++)
     {
        sum += func(a + (value_type((j * 2) - 1) * h));
     }

     const value_type I0 = I;
     I = (I / 2) + (h * sum);

     const value_type ratio     = I0 / I;
     const value_type delta     = ratio - 1;
     const value_type delta_abs = ((delta < 0) ? -delta : delta);

     if((k > 1U) && (delta_abs < atol))
     {
        break;
     }

     n *= 2U;
  }

  return I;
}

/**
  * see equation 26 in ContinuumI paper
  * \brief use this when you are in the current configuration and
  * \brief trying to go back to the reference configuration
  *
  * \tparam Ro, Ri, are desired radii to transition to given by user
  * \tparam R is the variable we are integrating in reference configuration
  * \tparam r external radius in current configuration
  * \tparam C1 material elasticity of inner IAB wall
  * \tparam C2 material elasticity of outer IAB wall
  */

template<typename value_type>
radial_stress_c2r<value_type>::radial_stress_c2r(const value_type& Ri, const value_type& Ro,
                          const value_type& ri,
                          const value_type& C1, const value_type& C2)
                          : Ri_(Ri), Ro_(Ro), ri_(ri), C1_(C1), C2_(C2)
                          {
                          }

template<typename value_type>
value_type radial_stress_c2r<value_type>::operator() (const value_type& R) const // see equation 25 in ContinuumI paper
{
  //return the integrand expression
  const value_type& r = get_r();
  return -1*(2*C1_*(1/r - std::pow(R, 6)/std::pow(r, 7)) \
        -2*C2_*(std::pow(R, 4)/std::pow(r, 5)-r/std::pow(R, 2)));
}

template<typename value_type>
inline value_type radial_stress_c2r<value_type>::get_r()
{
  return std::cbrt(std::pow(Ro_, 3) + std::pow(ri_, 3) - std::pow(Ri_, 3));
}

/**
  * see equation 25 in ContinuumI paper
  * \brief use this when you are in the reference configuration and
  * \brief trying to go to the current configuration
  *
  * \tparam ri, ro, are desired current configuration
  *         radii to transition to given by user
  * \tparam R is the variable to be found in Reference configuration
  * \tparam Ri internal radius in Reference configuration
  * \tparam C1 material elasticity of inner IAB wall
  * \tparam C2 material elasticity of outer IAB wall
  */

template<typename value_type>
radial_stress_r2c<value_type>::radial_stress_r2c(const value_type& ri, const value_type& ro,
                          const value_type& Ri,
                          const value_type& C1, const value_type& C2)
                          : ri_(ri), ro_(ro), Ri_(Ri), C1_(C1), C2_(C2)
                          { }


template<typename value_type>
value_type radial_stress_r2c<value_type>::operator() (const value_type& r) const // see equation 25 in ContinuumI paper
{
  //return the integrand expression
  const value_type& R = get_R();
  return -1*(2*C1_*(r/std::pow(R, 2) - std::pow(R, 4)/std::pow(r, 5)) \
        +2*C2_*(std::pow(r, 3)/std::pow(R, 4)-std::pow(R, 2)/std::pow(r, 3)));
}

template<typename value_type>
inline value_type radial_stress_r2c<value_type>::get_R()
{
  return std::cbrt(std::pow(ro_, 3) - std::pow(ri_, 3) + std::pow(Ri_, 3));
}

/**
  *see equation 27 in ContinuumI paper
  * \brief use this to calculate pressure in going from
  * \brief the reference configuration to the current configuration
  *
  * \tparam ri, ro, are desired current configuration
  *         radii to transition to given by user
  * \tparam R is the variable to be found in Reference configuration
  * \tparam Ri internal radius in Reference configuration
  * \tparam C1 material elasticity of inner IAB wall
  * \tparam C2 material elasticity of outer IAB wall
  */

template<typename value_type>
pressure_r2c<value_type>::pressure_r2c(const value_type& ri, const value_type& ro,
                          const value_type& Ri,
                          const value_type& C1, const value_type& C2)
                          : ri_(ri), ro_(ro), Ri_(Ri), C1_(C1), C2_(C2)
                          { }

template<typename value_type>
value_type pressure_r2c<value_type>::operator() (const value_type& r) const // see equation 25 in ContinuumI paper
{
  //return the integrand expression
  const value_type& R = get_R();
  return 2*C1_*(r/std::pow(R, 2) - std::pow(R, 4)/std::pow(r, 5)) \
        +2*C2_*(std::pow(r, 3)/std::pow(R, 4)-std::pow(R, 2)/std::pow(r, 3));
}

template<typename value_type>
inline value_type pressure_r2c<value_type>::get_R()
{
  return std::cbrt(std::pow(ro_, 3) - std::pow(ri_, 3) + std::pow(Ri_, 3));
}

/**
  *see equation 28 in ContinuumI paper
  * \brief use this to calculate pressure in going from
  * \brief the current configuration to the reference configuration
  *
  * \tparam ri, ro, are desired current configuration
  *         radii to transition to given by user
  * \tparam R is the variable to be found in Reference configuration
  * \tparam Ri internal radius in Reference configuration
  * \tparam C1 material elasticity of inner IAB wall
  * \tparam C2 material elasticity of outer IAB wall
  */
template<typename value_type>
pressure_c2r<value_type>::pressure_c2r(const value_type& Ri, const value_type& Ro,
                          const value_type& ri,
                          const value_type& C1, const value_type& C2)
                          : Ri_(Ri), Ro_(Ro), ri_(ri), C1_(C1), C2_(C2)
                          { }

template<typename value_type>
value_type pressure_c2r<value_type>::operator() (const value_type& R) const // see equation 25 in ContinuumI paper
{
  //return the integrand expression
  const value_type& r = get_r();
  return 2*C1_*(1/r - std::pow(R, 6)/std::pow(r, 7)) \
        -2*C2_*(std::pow(R, 4)/std::pow(r, 5)-r/std::pow(R, 2));
}

template<typename value_type>
inline value_type pressure_c2r<value_type>::get_r()
{
  return std::cbrt(std::pow(Ro_, 3) + std::pow(ri_, 3) - std::pow(Ri_, 3));
}
