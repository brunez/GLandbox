/***************************************************************************
*
* Copyright 2014 Bruno Ordozgoiti
* Distributed under GNU GPLv3
*
* This file is part of GLandbox.
*
* GLandbox is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* GLandbox is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with GLandbox. If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************/

#ifndef PARAM_H
#define PARAM_H

#include <ostream>
#include <sstream>

namespace gland{
  class Param{
  public:
    
    Param(){}
    explicit Param(const std::string &name):name_(name){}
    
    union{
      float f;
      int i;
      double d;
      bool b;
    }u;

    void operator =(float f);    
    void operator +=(float f);    
    void operator -=(float f);    
    float operator *(float f){return u.f*f;}    
    void operator =(int i);   
    void operator +=(int i);   
    void operator -=(int i);   
    void operator =(const double d);      
    void operator +=(const double d);      
    void operator -=(const double d);      
    void operator =(const bool b);
    operator bool() const {return u.b;}

    operator float() const {return u.f;}
    operator int() const {return u.i;}
    operator double() const {return u.d;}

    std::string name_;
    const unsigned short getMember() const;
  
  private:
    unsigned short member_;
               
  };
  
  std::ostream& operator << (std::ostream& stream, const gland::Param& p);
  
  float operator *(const float f, const gland::Param &p) ;
  int operator *(const int f, const gland::Param &p) ;

  double operator *(const double f, const gland::Param &p) ;


}
#endif
