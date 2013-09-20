//-----------------------------------------------------------------------------
//
//
//! OpenSteer -- Steering Behaviors for Autonomous Characters
//
//! Copyright (c) 2002-2005, Sony Computer Entertainment America
//! Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
//! Permission is hereby granted, free of charge, to any person obtaining a
//! copy of this software and associated documentation files (the "Software"),
//! to deal in the Software without restriction, including without limitation
//! the rights to use, copy, modify, merge, publish, distribute, sublicense,
//! and/or sell copies of the Software, and to permit persons to whom the
//! Software is furnished to do so, subject to the following conditions:
//
//! The above copyright notice and this permission notice shall be included in
//! all copies or substantial portions of the Software.
//
//! THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//! IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//! FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
//! THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//! LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//! FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//! DEALINGS IN THE SOFTWARE.
//
//
//-----------------------------------------------------------------------------
//
//
//! Color class and predefined colors.
//
//! May 05, 2005 bk:  created 
//
//
//-----------------------------------------------------------------------------


#ifndef OPENSTEER_COLOR_H
#define OPENSTEER_COLOR_H




namespace OpenSteer {
    
    //! Forward declaration. Full declaration in Vec3.h
    class Vec3;
    
    
	//! a simple rgba float class
    class Color {
    public:
        Color();
        explicit Color( float greyValue );
        Color( float rValue, float gValue, float bValue, float aValue = 1.0f );
        explicit Color( Vec3 const& vector );
        
        float r() const;
        float g() const;
        float b() const;
        float a() const;
        
        void setR( float value );
        void setG( float value );
        void setB( float value );
		void setA( float value );
        void set( float rValue, float gValue, float bValue, float aValue = 1.0f );
        
        Vec3 convertToVec3() const;
    
		//! this is necessary so that graphics API's such as DirectX
		//! requiring a pointer to colors can do their conversion
		//! without a lot of copying.
		float const* const colorFloatArray() const { return &r_; }

        Color& operator+=( Color const& other );
        
        /**
         * @todo What happens if color components become negative?
         */
        Color& operator-=( Color const& other );
        
        /**
         * @todo What happens if color components become negative?
         */
        Color& operator*=( float factor );
        
        /**
         * @todo What happens if color components become negative?
         */
        Color& operator/=( float factor );
        
        
    private:
        float r_;
        float g_;
        float b_;
		float a_;	//! provided for API's which require four components       

	public:
		 static const Color _gBlack;
		 static const Color _gWhite;

		 static const Color _gRed; 
		 static const Color _gGreen;
		 static const Color _gBlue;
		 static const Color _gYellow;
		 static const Color _gCyan;
		 static const Color _gMagenta;
		 static const Color _gOrange;

		 static const Color _gDarkRed;
		 static const Color _gDarkGreen;
		 static const Color _gDarkBlue;
		 static const Color _gDarkYellow;
		 static const Color _gDarkCyan;
		 static const Color _gDarkMagenta;
		 static const Color _gDarkOrange;

		 static const Color _gGray10;
		 static const Color _gGray20;
		 static const Color _gGray30;
		 static const Color _gGray40;
		 static const Color _gGray50;
		 static const Color _gGray60;
		 static const Color _gGray70;
		 static const Color _gGray80;


    }; //! class Color
    
    
    Color operator+( Color const& lhs, Color const& rhs );
    
    /**
     * @todo What happens if color components become negative?
     */
    Color operator-( Color const& lhs, Color const& rhs );
    
    /**
     * @todo What happens if color components become negative?
     */
    Color operator*( Color const& lhs, float rhs );
    
    /**
     * @todo What happens if color components become negative?
     */
    Color operator*( float lhs, Color const& rhs );
    
    /**
     * @todo What happens if color components become negative?
     */
    Color operator/( Color const& lhs, float rhs );
    
    
    Color grayColor( float value );
    
    
} //! namespace OpenSteer

#define gBlack OpenSteer::Color::_gBlack
#define gWhite OpenSteer::Color::_gWhite
#define gRed OpenSteer::Color::_gRed
#define gGreen OpenSteer::Color::_gGreen
#define gBlue OpenSteer::Color::_gBlue
#define gYellow OpenSteer::Color::_gYellow
#define gCyan OpenSteer::Color::_gCyan
#define gMagenta OpenSteer::Color::_gMagenta
#define gOrange OpenSteer::Color::_gOrange
#define gDarkRed OpenSteer::Color::_gDarkRed
#define gDarkGreen OpenSteer::Color::_gDarkGreen
#define gDarkBlue OpenSteer::Color::_gDarkBlue
#define gDarkYellow OpenSteer::Color::_gDarkYellow
#define gDarkCyan OpenSteer::Color::_gDarkCyan
#define gDarkMagenta OpenSteer::Color::_gDarkMagenta
#define gDarkOrange OpenSteer::Color::_gDarkOrange

#define gGray10 OpenSteer::Color::_gGray10
#define gGray20 OpenSteer::Color::_gGray20
#define gGray30 OpenSteer::Color::_gGray30
#define gGray40 OpenSteer::Color::_gGray40
#define gGray50 OpenSteer::Color::_gGray50
#define gGray60 OpenSteer::Color::_gGray60
#define gGray70 OpenSteer::Color::_gGray70
#define gGray80 OpenSteer::Color::_gGray80
#define gGray90 OpenSteer::Color::_gGray90

#endif //! OPENSTEER_COLOR_H
