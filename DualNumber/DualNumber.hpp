#pragma once

#include <complex>
#include <iostream>

namespace DualNumbers {

	/**
	* @brief �o�ΐ��i��d���j��tratis
	* @tparam T �C�ӂ̓�d���̎����^
	*/
	template<typename T>
	struct dual_number_traits {
		static constexpr auto a(const T& val) {
			return val.a();
		}

		static constexpr auto b(const T& val) {
			return val.b();
		}
	};

	/**
	* @brief std::complex��ΏۂƂ���tratis����
	*/
	template<typename T>
	struct dual_number_traits<std::complex<T>> {

		static constexpr auto a(const std::complex<T>& val) {
			return val.real();
		}

		static constexpr auto b(const std::complex<T>& val) {
			return val.imag();
		}
	};


	/**
	* @brief �o�ΐ��i��d���j�̎���
	* @tparam T �l�^�Adouble�Ɠ������삪�ł���^
	*/
	template<typename T>
	struct dual {
		using this_type  = dual<T>;
		using value_type = T;

		/**
		* �o�ΐ��̗댳
		* @return 0 + 0��
		*/
		static constexpr this_type Zero() {
			return this_type{ T(0.0), T(0.0) };
		}

		/**
		* �o�ΐ��̉��@�P�ʌ�
		* @return 0 + 0��
		*/
		static constexpr this_type ID_Add() {
			return Zero();
		}

		/**
		* �o�ΐ��̏�@�P�ʌ�
		* @return 1 + 0��
		*/
		static constexpr this_type ID_Mul() {
			return this_type{ T(0.0), T(0.0) };
		}

		/**
		* �f�t�H���g�R���X�g���N�^
		*/
		constexpr dual()
			: m_a{ 0.0 }
			, m_b{ 0.0 }
		{}

		/**
		* ��{�R���X�g���N�^�A�l�����č\�z
		*/
		constexpr dual(T a, T b = T(0.0))
			: m_a{ a }
			, m_b{ b }
		{}

		/**
		* ���̑o�ΐ���������̕ϊ��R���X�g���N�^
		* @brief ���̌^�ɓK������dual_number_traits<T>�̓��ꉻ���K�v
		* @param other �C�ӂ̑o�ΐ�
		*/
		template<typename OtherDual>
		constexpr dual(const OtherDual& other)
			: m_a{ T{dual_number_traits<OtherDual>::a(other)} }
			, m_b{ T{dual_number_traits<OtherDual>::b(other)} }
		{}

		constexpr dual(const this_type& other) = default;
		constexpr dual(this_type&& other) = default;
		
		constexpr this_type& operator=(const this_type& other) & = default;
		constexpr this_type& operator=(this_type&& other) & = default;

		constexpr operator T() const {
			return m_a;
		}

		constexpr this_type operator+() const {
			return *this;
		}

		constexpr this_type operator-() const {
			return this_type{ -m_a,-m_b };
		}

		constexpr bool operator==(const this_type& rhs) const {
			//return m_a == rhs.m_a && m_b == rhs.m_b;
			return std::tie(m_a, m_b) == std::tie(rhs.m_a, rhs.m_b);
		}

		constexpr bool operator<(const this_type& rhs) const {
			return std::tie(m_a, m_b) < std::tie(rhs.m_a, rhs.m_b);
		}

		constexpr this_type& operator++() {
			++m_a;
			return *this;
		}

		constexpr this_type& operator--() {
			--m_a;
			return *this;
		}

		constexpr this_type& operator+=(const this_type& rhs) {
			m_a += rhs.m_a;
			m_b += rhs.m_b;

			return *this;
		}

		constexpr this_type& operator+=(const T rhs) {
			m_a += rhs;

			return *this;
		}

		constexpr this_type& operator-=(const this_type& rhs) {
			m_a -= rhs.m_a;
			m_b -= rhs.m_b;

			return *this;
		}

		constexpr this_type& operator-=(const T rhs) {
			m_a -= rhs;

			return *this;
		}

		constexpr this_type& operator*=(const this_type& rhs) {
			//(a+b��)*(c+d��) = ac + (ad + bc)��
			m_b *= rhs.m_a;
			m_b += m_a * rhs.m_b;
			m_a *= rhs.m_a;

			return *this;
		}

		constexpr this_type& operator*=(const T rhs) {
			m_b *= rhs;
			m_a *= rhs;

			return *this;
		}

		constexpr this_type& operator/=(const this_type& rhs) {
			// (a+b��)/(c+d��) = a/c + (-ad + bc)��/c^2
			m_b *= rhs.m_a;
			m_b -= m_a * rhs.m_b;
			m_b /= rhs.m_a * rhs.m_a;
			m_a /= rhs.m_a;

			return *this;
		}

		constexpr this_type& operator/=(const T rhs) {
			m_a /= rhs;
			m_b /= rhs;

			return *this;
		}

		/**
		* ���݂̒l�����̋t���ɂ���
		* @brief ���ݕێ�����l�����̐ς̋t���ɍX�V����
		* @detail ����(a)���[���łȂ���
		*/
		constexpr void inverse() {
			this->inverted();
		}

		/**
		* ���݂̒l�����̋����ɂ���
		* @brief ���ݕێ�����l�����̋���o�ΐ��ɂ���
		*/
		constexpr void conjugate() {
			this->conjugated();
		}

		/**
		* �o�ΐ��̋t���𓾂�
		* @brief �w�肳�ꂽ�o�ΐ��̋t�������߂�
		* @param dual ����(a)���[���łȂ��o�ΐ�
		* @return �t��(1/a - b/a^2��)
		*/
		constexpr this_type& inverted() {
			//d^-1 = 1/a - b/a^2
			m_a = T(1.0) / m_a;
			m_b /= -(m_a* m_a);
			return *this;
		}

		/**
		* �o�ΐ��̋����𓾂�
		* @brief �w�肳�ꂽ�o�ΐ��̋��������߂�
		* @param dual ����(a)���[���łȂ��o�ΐ�
		* @return ����o�ΐ�(a - b��)
		*/
		constexpr this_type& conjugated() {
			m_b = -m_b;
			return *this;
		}
		
		/**
		* �������擾����
		* @return �����̒l
		*/
		constexpr T a() const {
			return m_a;
		}

		/**
		* �������擾����
		* @return �����̒l
		*/
		constexpr T b() const {
			return m_b;
		}

	private:
		value_type m_a;
		value_type m_b;
	};


	template<typename T>
	constexpr bool operator!=(const dual<T>& lhs, const dual<T>& rhs) {
		return !(lhs == rhs);
	}

	template<typename T>
	constexpr bool operator<=(const dual<T>& lhs, const dual<T>& rhs) {
		return (lhs == rhs) || (lhs < rhs);
	}

	template<typename T>
	constexpr bool operator>(const dual<T>& lhs, const dual<T>& rhs) {
		return rhs < lhs;
	}

	template<typename T>
	constexpr bool operator>=(const dual<T>& lhs, const dual<T>& rhs) {
		return (lhs == rhs) || (lhs > rhs);
	}

	template<typename T>
	constexpr auto operator++(dual<T>& dual, int) {
		auto copy = dual;
		++dual;
		return copy;
	}

	template<typename T>
	constexpr auto operator--(dual<T>& dual, int) {
		auto copy = dual;
		--dual;
		return copy;
	}

	template<typename T>
	constexpr auto operator+(const dual<T>& lhs, const dual<T>& rhs) {
		return dual<T>{lhs} += rhs;
	}

	template<typename T>
	constexpr auto operator+(const dual<T>& lhs, const T rhs) {
		return dual<T>{lhs} += rhs;
	}

	template<typename T>
	constexpr auto operator+(const T lhs, const dual<T>& rhs) {
		return dual<T>{rhs} += lhs;
	}

	template<typename T>
	constexpr auto operator*(const dual<T>& lhs, const dual<T>& rhs) {
		return dual<T>{lhs} *= rhs;
	}

	template<typename T>
	constexpr auto operator*(const dual<T>& lhs, const T rhs) {
		return dual<T>{lhs} *= rhs;
	}

	template<typename T>
	constexpr auto operator*(const T lhs, const dual<T>& rhs) {
		return dual<T>{rhs} *= lhs;
	}

	template<typename T>
	constexpr auto operator-(const dual<T>& lhs, const dual<T>& rhs) {
		return dual<T>{lhs} -= rhs;
	}

	template<typename T>
	constexpr auto operator-(const dual<T>& lhs, const T rhs) {
		return dual<T>{lhs} -= rhs;
	}

	template<typename T>
	constexpr auto operator-(const T lhs, const dual<T>& rhs) {
		return dual<T>{lhs} -= rhs;
	}
	
	template<typename T>
	constexpr auto operator/(const dual<T>& lhs, const dual<T>& rhs) {
		return dual<T>{lhs} /= rhs;
	}

	template<typename T>
	constexpr auto operator/(const dual<T>& lhs, const T rhs) {
		return dual<T>{lhs} /= rhs;
	}

	template<typename T>
	constexpr auto operator/(const T lhs, const dual<T>& rhs) {
		return dual<T>{lhs} /= rhs;
	}

	template<typename Char, typename Traits, typename T>
	std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<char, Traits>& ostream, const dual<T>& rhs) {
		ostream << rhs.a() << " + " << rhs.b() << "e";
		return ostream;
	}

	constexpr dual<double> operator"" _d(long double real) {
		return {static_cast<double>(real), 0.0};
	}

	constexpr dual<double> operator"" _eps(long double eps) {
		return {0.0, static_cast<double>(eps)};
	}

	/**
	* �o�ΐ��̋t���𓾂�
	* @brief �w�肳�ꂽ�o�ΐ��̋t�������߂�
	* @param dual ����(a)���[���łȂ��o�ΐ�
	* @return �t��(1/a - b/a^2��)
	*/
	template<typename T>
	constexpr auto inverted(dual<T>& dual) {
		dual.inverse();
		return dual;
	}

	/**
	* �o�ΐ��̋����𓾂�
	* @brief �w�肳�ꂽ�o�ΐ��̋��������߂�
	* @param dual ����(a)���[���łȂ��o�ΐ�
	* @return ����o�ΐ�(a - b��)
	*/
	template<typename T>
	constexpr auto conjugated(dual<T>& dual) {
		dual.conjugate();
		return dual;
	}

	using dual_f  = dual<float>;
	using dual_d  = dual<double>;
	using dual_ld = dual<long double>;

	inline namespace cmath {

		namespace Constant {
			template<typename T>
			constexpr T loge_2 = static_cast<T>(0.693147180559945309417232121458);

			template<typename T>
			constexpr T loge_10 = static_cast<T>(2.30258509299404568401799145468);
		}

		template<typename T>
		auto atan2(const dual<T>& y, const dual<T>& x) {
			using std::atan2;

			auto sumsq_inv = T(1.0) / (x.a() * x.a() + y.a() * y.a());
			return dual<T>{atan2(y.a(), x.a()), sumsq_inv*(-y.a()*x.b() + x.a()*y.b())};
		}

		template<typename T>
		auto pow(const dual<T>& f, const dual<T>& y) {
			using std::pow;

			auto real = pow(f.a(), y.b());
			auto dpow_y = y.a() * pow(f.a(), y.a() - T(1.0));
			auto dpow_f = real * log(f.a());
			return dual<T>{real, dpow_y * f.b() + dpow_f * y.b()};
		}

		template<typename Exponent, typename T>
		auto pow(Exponent f, const dual<T>& y) {
			using std::pow;
			using std::log;

			auto tmp = pow(f, y.a());
			return dual<T>{tmp, y.b() * tmp * log(f)};
		}

		template<typename T, typename Exponent>
		auto pow(const dual<T>& d, Exponent y) {
			using std::pow;
			return dual<T>{pow(d.a(), y), static_cast<T>(y)*d.b()*pow(d.a(), y - T(1.0))};
		}

		template<typename T>
		auto sqrt(const dual<T>& d) {
			using std::sqrt;
			auto sqrt_a = sqrt(d.a());
			return dual<T>{sqrt_a, d.b() / (sqrt_a + sqrt_a)};
		}

		template<typename T>
		auto cbrt(const dual<T>& d) {
			using std::cbrt;
			auto cbrt_a = cbrt(d.a());
			cbrt_a *= cbrt_a;
			return dual<T>{cbrt_a, d.b() / (cbrt_a + cbrt_a + cbrt_a)};
		}

		template<typename T>
		auto sin(const dual<T>& d) {
			using std::sin;
			using std::cos;

			return dual<T>{sin(d.a()), d.b()*cos(d.a())};
		}

		template<typename T>
		auto cos(const dual<T>& d) {
			using std::sin;
			using std::cos;

			return dual<T>{cos(d.a()), -d.b()*sin(d.a())};
		}

		template<typename T>
		auto tan(const dual<T>& d) {
			using std::cos;
			using std::tan;

			auto cos_a = cos(d.a());
			return dual<T>{tan(d.a()), d.b() / (cos_a*cos_a)};
		}

		template<typename T>
		auto asin(const dual<T>& d) {
			using std::asin;
			using std::sqrt;

			return dual<T>{asin(d.a()), d.b() / sqrt(T(1.0) -d.a()*d.a())};
		}

		template<typename T>
		auto acos(const dual<T>& d) {
			using std::acos;
			using std::sqrt;

			return dual<T>{acos(d.a()), -d.b() / sqrt(T(1.0) -d.a()*d.a())};
		}

		template<typename T>
		auto atan(const dual<T>& d) {
			using std::atan;

			return dual<T>{atan(d.a()), d.b() / (T(1.0) + d.a()*d.a())};
		}

		template<typename T>
		auto sinh(const dual<T>& d) {
			using std::sinh;
			using std::cosh;

			return dual<T>{sinh(d.a()), d.b()* cosh(d.a()) };
		}

		template<typename T>
		auto cosh(const dual<T>& d) {
			using std::sinh;
			using std::cosh;

			return dual<T>{cosh(d.a()), d.b()* sinh(d.a()) };
		}

		template<typename T>
		auto tanh(const dual<T>& d) {
			using std::tanh;
			using std::cosh;

			auto cosh_a = cosh(d.a());
			return dual<T>{tanh(d.a()), d.b() / (cosh_a*cosh_a)};
		}

		template<typename T>
		auto asinh(const dual<T>& d) {
			using std::asinh;
			using std::sqrt;

			return dual<T>{asinh(d.a()), d.b() / sqrt(T(1.0) +d.a()*d.a())};
		}

		template<typename T>
		auto acosh(const dual<T>& d) {
			using std::acosh;
			using std::sqrt;

			return dual<T>{acosh(d.a()), d.b() / sqrt(d.a()*d.a() - T(1.0))};
		}

		template<typename T>
		auto atanh(const dual<T>& d) {
			using std::atanh;

			return dual<T>{atanh(d.a()), d.b() / (T(1.0) -d.a()*d.a())};
		}

		template<typename T>
		auto exp(const dual<T>& d) {
			using std::exp;

			auto f = exp(d.a());
			return dual<T>{f, d.b()*f};
		}

		template<typename T>
		auto exp2(const dual<T>& d) {
			using std::exp2;

			auto f = exp2(d.a());
			return dual<T>{f, d.b()*f*Constant::loge_2<T>};
		}

		template<typename T>
		auto expm1(const dual<T>& d) {
			using std::exp;
			using std::expm1;

			return dual<T>{expm1(d.a()), d.b()*exp(d.a())};
		}

		template<typename T>
		auto log(const dual<T>& d) {
			using std::log;

			return dual<T>{log(d.a()), d.b() / d.a()};
		}

		template<typename T>
		auto log1p(const dual<T>& d) {
			using std::log1p;

			return dual<T>{log1p(d.a()), d.b() / (T(1.0) + d.a())};
		}

		template<typename T>
		auto log10(const dual<T>& d) {
			using std::log10;

			return dual<T>{log10(d.a()), d.b() / (d.a() * Constant::loge_10<T>)};
		}

		template<typename T>
		auto log2(const dual<T>& d) {
			using std::log10;

			return dual<T>{log2(d.a()), d.b() / (d.a() * Constant::loge_2<T>)};
		}


		/**
		* �e��x�b�Z���֐��Ƃ��̔������v�Z����
		* @tparam T nu�̌^
		* @tparam T2 ���͑o�ΐ��̓����l�^
		* @tparam BesselFunctor �e��x�b�Z���֐������b�v�����t�@���N�^
		* @param nu �e��~���֐��̃�(v)
		* @param x ���͑o�ΐ�
		* @param bessel �e��x�b�Z���֐������b�v�����t�@���N�^�̎���
		*/
		template<typename T, typename T2, typename BesselFunctor>
		static auto calculateBesselFunctions(T nu, const dual<T2>& x, BesselFunctor&& bessel) {
			using ReturnType = decltype(bessel(T(0.0), x.a()));

			if (nu == T{ 0.0 }) {
				// Z0'(x) = -Z1(x)�AZ=�C�ӂ̉~���֐��i�x�b�Z���A�m�C�}���A�n���P���j
				return dual<ReturnType>{bessel(T(0.0), x.a()), -x.b() * bessel(T(0.0), x.a())};
			}
			else {
				// Zn'(x) = 0.5*(Zn-1(x) - Zn+1(x))
				return dual<ReturnType>{bessel(nu, x.a()), T2(0.5) *x.b() * (bessel(nu - T(1.0), x.a()) - bessel(nu + T(1.0), x.a()))};
			}
		}

		/**
		* �e��ό`�x�b�Z���֐��Ƃ��̔������v�Z����
		* @tparam T nu�̌^
		* @tparam T2 ���͑o�ΐ��̓����l�^
		* @tparam BesselFunctor �e��ό`�x�b�Z���֐������b�v�����t�@���N�^
		* @param nu �e��~���֐��̃�(v)
		* @param x ���͑o�ΐ�
		* @param bessel �e��ό`�x�b�Z���֐������b�v�����t�@���N�^�̎���
		* @return �ό`�x�b�Z���֐��ɒʂ����o�ΐ��A����̏ꍇ�͔����̕������قȂ�
		*/
		template<typename T, typename T2, typename BesselFunctor>
		static auto calculateModifiedBesselFunctions(T nu, const dual<T2>& x, BesselFunctor&& bessel) {
			using ReturnType = decltype(bessel(T(0.0), x.a()));

			if (nu == T(0.0)) {
				// Z0'(x) = Z1(x)�AZ=�C�ӂ̕ό`�x�b�Z���֐�
				return dual<ReturnType>{bessel(T(0.0), x.a()), x.b() * bessel(T(1.0), x.a())};
			}
			else {
				// Zn'(x) = 0.5*(Zn-1(x) + Zn+1(x))
				return dual<ReturnType>{bessel(nu, x.a()), T2(0.5) *x.b() * (bessel(nu - T(1.0), x.a()) + bessel(nu + T(1.0), x.a()))};
			}
		}

		/**
		* ����x�b�Z���֐��Afloat�p
		* @tparam T ���͑o�ΐ��̓����l�^
		* @param nu �e��~���֐��̃�(v)
		* @param x ���͑o�ΐ�
		*/
		template<typename T>
		auto cyl_bessel_jf(float nu, const dual<T>& x) {
			using std::cyl_bessel_jf;

			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_bessel_jf(nu, x); });
		}

		/**
		* ����x�b�Z���֐��Adouble�p
		* @tparam T ���͑o�ΐ��̓����l�^
		* @param nu �e��~���֐��̃�(v)
		* @param x ���͑o�ΐ�
		*/
		template<typename T>
		auto cyl_bessel_j(double nu, const dual<T>& x) {
			using std::cyl_bessel_j;

			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_bessel_j(nu, x); });
		}

		/**
		* ����x�b�Z���֐��Along double�p
		* @tparam T ���͑o�ΐ��̓����l�^
		* @param nu �e��~���֐��̃�(v)
		* @param x ���͑o�ΐ�
		*/
		template<typename T>
		auto cyl_bessel_jl(long double nu, const dual<T>& x) {
			using std::cyl_bessel_jl;

			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_bessel_jl(nu, x); });
		}

		/**
		* ����x�b�Z���֐�(�m�C�}���֐�)�Afloat�p
		* @tparam T ���͑o�ΐ��̓����l�^
		* @param nu �e��~���֐��̃�(v)
		* @param x ���͑o�ΐ�
		*/
		template<typename T>
		auto cyl_neumannf(float nu, const dual<T>& x) {
			using std::cyl_neumannf;

			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_neumannf(nu, x); });
		}

		/**
		* ����x�b�Z���֐�(�m�C�}���֐�)�Adouble�p
		* @tparam T ���͑o�ΐ��̓����l�^
		* @param nu �e��~���֐��̃�(v)
		* @param x ���͑o�ΐ�
		*/
		template<typename T>
		auto cyl_neumann(double nu, const dual<T>& x) {
			using std::cyl_neumann;

			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_neumann(nu, x); });
		}

		/**
		* ����x�b�Z���֐�(�m�C�}���֐�)�Along double�p
		* @tparam T ���͑o�ΐ��̓����l�^
		* @param nu �e��~���֐��̃�(v)
		* @param x ���͑o�ΐ�
		*/
		template<typename T>
		auto cyl_neumannl(long double nu, const dual<T>& x) {
			using std::cyl_neumannl;

			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_neumannl(nu, x); });
		}

		/**
		* ��O��x�b�Z���֐�(�n���P���֐��̈�A�����̕������v���X)�Afloat�p
		* @tparam T ���͑o�ΐ��̓����l�^
		* @param nu �e��~���֐��̃�(v)
		* @param x ���͑o�ΐ�
		*/
		auto cyl_hankel_1f(float nu, const dual<float>& x) {
			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return std::complex<float>{std::cyl_bessel_jf(nu, x), std::cyl_neumannf(nu, x) }; });
		}

		/**
		* ��O��x�b�Z���֐�(�n���P���֐��̈�A�����̕������}�C�i�X)�Afloat�p
		* @tparam T ���͑o�ΐ��̓����l�^
		* @param nu �e��~���֐��̃�(v)
		* @param x ���͑o�ΐ�
		*/
		auto cyl_hankel_2f(float nu, const dual<float>& x) {
			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return std::complex<float>{std::cyl_bessel_jf(nu, x), -std::cyl_neumannf(nu, x) }; });
		}

		/**
		* ��O��x�b�Z���֐�(�n���P���֐��̈�A�����̕������v���X)�Adouble�p
		* @tparam T ���͑o�ΐ��̓����l�^
		* @param nu �e��~���֐��̃�(v)
		* @param x ���͑o�ΐ�
		*/
		auto cyl_hankel_1(double nu, const dual<double>& x) {
			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return std::complex<double>{std::cyl_bessel_j(nu, x), std::cyl_neumann(nu, x) }; });
		}

		/**
		* ��O��x�b�Z���֐�(�n���P���֐��̈�A�����̕������}�C�i�X)�Adouble�p
		* @tparam T ���͑o�ΐ��̓����l�^
		* @param nu �e��~���֐��̃�(v)
		* @param x ���͑o�ΐ�
		*/
		auto cyl_hankel_2(double nu, const dual<double>& x) {
			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return std::complex<double>{std::cyl_bessel_j(nu, x), -std::cyl_neumann(nu, x) }; });
		}

		/**
		* ��O��x�b�Z���֐�(�n���P���֐��̈�A�����̕������v���X)�Along double�p
		* @tparam T ���͑o�ΐ��̓����l�^
		* @param nu �e��~���֐��̃�(v)
		* @param x ���͑o�ΐ�
		*/
		auto cyl_hankel_1l(long double nu, const dual<long double>& x) {
			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return std::complex<long double>{std::cyl_bessel_jl(nu, x), std::cyl_neumannl(nu, x) }; });
		}

		/**
		* ��O��x�b�Z���֐�(�n���P���֐��̈�A�����̕������}�C�i�X)�Along double�p
		* @tparam T ���͑o�ΐ��̓����l�^
		* @param nu �e��~���֐��̃�(v)
		* @param x ���͑o�ΐ�
		*/
		auto cyl_hankel_2l(long double nu, const dual<long double>& x) {
			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return std::complex<long double>{std::cyl_bessel_jl(nu, x), -std::cyl_neumannl(nu, x) }; });
		}

		/**
		* ����ό`�x�b�Z���֐��Afloat�p
		* @tparam T ���͑o�ΐ��̓����l�^
		* @param nu �e��~���֐��̃�(v)
		* @param x ���͑o�ΐ�
		*/
		template<typename T>
		auto cyl_bessel_if(float nu, const dual<T>& x) {
			using std::cyl_bessel_if;

			return calculateModifiedBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_bessel_if(nu, x); });
		}

		/**
		* ����ό`�x�b�Z���֐��Adouble�p
		* @tparam T ���͑o�ΐ��̓����l�^
		* @param nu �e��~���֐��̃�(v)
		* @param x ���͑o�ΐ�
		*/
		template<typename T>
		auto cyl_bessel_i(double nu, const dual<T>& x) {
			using std::cyl_bessel_i;

			return calculateModifiedBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_bessel_i(nu, x); });
		}

		/**
		* ����ό`�x�b�Z���֐��Along double�p
		* @tparam T ���͑o�ΐ��̓����l�^
		* @param nu �e��~���֐��̃�(v)
		* @param x ���͑o�ΐ�
		*/
		template<typename T>
		auto cyl_bessel_il(long double nu, const dual<T>& x) {
			using std::cyl_bessel_il;

			return calculateModifiedBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_bessel_il(nu, x); });
		}

		/**
		* ����ό`�x�b�Z���֐��Afloat�p
		* @tparam T ���͑o�ΐ��̓����l�^
		* @param nu �e��~���֐��̃�(v)
		* @param x ���͑o�ΐ�
		*/
		template<typename T>
		auto cyl_bessel_kf(float nu, const dual<T>& x) {
			using std::cyl_bessel_kf;

			dual<T> d = calculateModifiedBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_bessel_kf(nu, x); });
			return dual<T>{d.a(), -d.b()};
		}

		/**
		* ����ό`�x�b�Z���֐��Adouble�p
		* @tparam T ���͑o�ΐ��̓����l�^
		* @param nu �e��~���֐��̃�(v)
		* @param x ���͑o�ΐ�
		*/
		template<typename T>
		auto cyl_bessel_k(double nu, const dual<T>& x) {
			using std::cyl_bessel_k;

			dual<T> d = calculateModifiedBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_bessel_k(nu, x); });
			return dual<T>{d.a(), -d.b()};
		}

		/**
		* ����ό`�x�b�Z���֐��Along double�p
		* @tparam T ���͑o�ΐ��̓����l�^
		* @param nu �e��~���֐��̃�(v)
		* @param x ���͑o�ΐ�
		*/
		template<typename T>
		auto cyl_bessel_kl(long double nu, const dual<T>& x) {
			using std::cyl_bessel_kl;

			dual<T> d = calculateModifiedBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_bessel_kl(nu, x); });
			return dual<T>{d.a(), -d.b()};
		}
	}
}