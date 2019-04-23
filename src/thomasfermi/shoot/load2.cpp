﻿/*! \file load2.cpp
    \brief y(x)の初期関数y0(x)の、原点と端点における
    関数値とその微分値を求めるクラスの実装
    Copyright © 2014-2019 @dc1394 All Rights Reserved.

    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation; either version 3 of the License, or (at your option)
    any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
    more details.

    You should have received a copy of the GNU General Public License along
    with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "load2.h"
#include <array>				// for std:array
#include <cmath>				// for std::pow
#include <boost/assert.hpp>		// for BOOST_ASSERT

namespace thomasfermi {
	namespace shoot {
        // #region コンストラクタ

		load2::load2() :
			acc_(gsl_interp_accel_alloc(), gsl_interp_accel_free),
			spline_(gsl_spline_alloc(gsl_interp_cspline, load2::XYSIZE), gsl_spline_free)
		{
            // 正規表現 = "  ([\d\.]+\, )+[\d\.]+\,"→"$1"

            // y(x)の近似値のx方向メッシュ
            std::array<double, load2::XYSIZE> const x = 
			{
				0.000, 0.010, 0.020, 0.030, 0.040, 0.050,
				0.060, 0.080, 0.100, 0.150, 0.200, 0.250,
				0.292, 0.300, 0.333, 0.350, 0.375, 0.400,
				0.417, 0.458, 0.500, 0.542, 0.584, 0.600,
				0.625, 0.667, 0.700, 0.709, 0.750, 0.792,
				0.800, 0.833, 0.875, 0.900, 0.917, 0.958,
				1.000, 1.042, 1.083, 1.100, 1.125, 1.167,
				1.200, 1.208, 1.250, 1.300, 1.400, 1.458,
				1.500, 1.600, 1.667, 1.700, 1.800, 1.875,
				1.900, 2.000, 2.083, 2.200, 2.292, 2.400,
				2.500, 2.600, 2.708, 2.800, 2.918, 3.000,
				3.125, 3.200, 3.333, 3.400, 3.500, 3.542,
				3.600, 3.750, 3.800, 3.960, 4.000, 4.167,
				4.375, 4.583, 4.792, 5.000, 5.209, 5.418,
				5.625, 5.834, 6.042, 6.250, 6.458, 6.667,
				6.875, 7.083, 7.292, 7.500, 7.708, 7.917,
				8.125, 8.333, 8.542, 8.750, 8.958, 9.167,
				9.375, 9.583, 9.792, 10.00, 10.22, 10.44,
				10.67, 10.92, 11.16, 11.43, 11.72, 12.01,
				12.31, 12.63, 12.97, 13.33, 13.72, 14.12,
				14.55, 15.01, 15.48, 16.00, 16.56, 17.14,
				17.78, 18.46, 19.20, 20.00, 20.87, 21.82,
				22.85, 24.00, 25.26, 26.67, 28.24, 30.00,
				32.00, 34.00, 34.29, 36.00, 36.92, 38.00,
				40.00, 45.00, 50.00, 55.00, 60.00, 65.00
			};

            // y(x)の近似値のyの数表
            // 参考:
            // E. U. Condon, Halis Odabasi. Atomic Structure, Cambridge University Press, Cambridge, 1980
			std::array<double, load2::XYSIZE> const y =
			{
				1.000, 0.985, 0.972, 0.959, 0.947, 0.935,
				0.924, 0.902, 0.882, 0.835, 0.793, 0.755,
				0.727, 0.721, 0.700, 0.691, 0.675, 0.660,
				0.651, 0.627, 0.607, 0.582, 0.569, 0.562,
				0.552, 0.535, 0.521, 0.518, 0.502, 0.488,
				0.485, 0.475, 0.461, 0.453, 0.449, 0.436,
				0.425, 0.414, 0.406, 0.398, 0.393, 0.382,
				0.375, 0.374, 0.364, 0.353, 0.333, 0.322,
				0.315, 0.297, 0.287, 0.283, 0.268, 0.259,
				0.255, 0.244, 0.234, 0.221, 0.212, 0.202,
				0.193, 0.185, 0.176, 0.170, 0.162, 0.157,
				0.150, 0.145, 0.138, 0.134, 0.130, 0.127,
				0.125, 0.118, 0.116, 0.110, 0.108, 0.102,
				0.0956, 0.0895, 0.0837, 0.0788, 0.0739, 0.0695,
				0.0656, 0.0619, 0.0587, 0.0554, 0.0526, 0.0500,
				0.0473, 0.0450, 0.0430, 0.0408, 0.0389, 0.0371,
				0.0355, 0.0340, 0.0321, 0.0310, 0.0298, 0.0287,
				0.0275, 0.0265, 0.0255, 0.0244, 0.0235, 0.0225,
				0.0216, 0.0206, 0.0198, 0.0189, 0.0180, 0.0171,
				0.0163, 0.0155, 0.0147, 0.0139, 0.0131, 0.0123,
				0.0116, 0.0109, 0.0102, 0.0094, 0.0088, 0.0081,
				0.0075, 0.0069, 0.0064, 0.0058, 0.0053, 0.0048,
				0.0043, 0.0038, 0.0034, 0.0030, 0.0026, 0.0022,
				0.0019, 0.0017, 0.0016, 0.0015, 0.0011, 0.0011,
				0.0011, 0.00079, 0.00063, 0.00049, 0.00039, 0.00032
			};
            
			BOOST_ASSERT(x.size() == y.size());
            
			gsl_spline_init(spline_.get(), x.data(), y.data(), x.size());
		}

        // #endregion publicメンバ関数

        // #region public staticメンバ関数

        double load2::dy0(double x) noexcept
        {
            auto const a = -3.0 * std::pow(load2::K, 3.0 / load2::LAMBDA) * std::pow(x, 3.0 / load2::LAMBDA - 1.0);
            return a * std::pow((1.0 + std::pow(load2::K * x, 3.0 / load2::LAMBDA)), -load2::LAMBDA - 1.0);
        }
        
        double load2::y0(double x) noexcept
        {
            return std::pow((1.0 + std::pow(load2::K * x, 3.0 / load2::LAMBDA)), -load2::LAMBDA);
        }

        // #endregion private staticメンバ関数
        
        // #region publicメンバ関数

        double load2::make_v2(double x2) const noexcept
        {
            return x2 > load2::THRESHOLD ? load2::dy0(x2) : gsl_spline_eval_deriv(spline_.get(), x2, acc_.get());
        }

        shootfunc::state_type load2::operator()(double x2, double v2) const
        {
			return { (x2 > load2::THRESHOLD ? load2::y0(x2) : gsl_spline_eval(spline_.get(), x2, acc_.get())), v2 };
        }

        // #endregion publicメンバ関数
	}
}

