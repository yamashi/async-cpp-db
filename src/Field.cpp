#include "Field.hpp"

template<>
std::optional<uint64_t> Async::Field::As<uint64_t>() const noexcept
{
	try
	{
		return std::stoull(m_data);
	}
	catch (...)
	{
		return std::nullopt;
	}
}

template<>
std::optional<int64_t> Async::Field::As<int64_t>() const noexcept
{
	try
	{
		return std::stoll(m_data);
	}
	catch (...)
	{
		return std::nullopt;
	}
}

template<>
std::optional<uint32_t> Async::Field::As<uint32_t>() const noexcept
{
	try
	{
		return std::stoul(m_data);
	}
	catch (...)
	{
		return std::nullopt;
	}
}

template<>
std::optional<int32_t> Async::Field::As<int32_t>() const noexcept
{
	try
	{
		return std::stol(m_data);
	}
	catch (...)
	{
		return std::nullopt;
	}
}

template<>
std::optional<double> Async::Field::As<double>() const noexcept
{
	try
	{
		return std::stod(m_data);
	}
	catch (...)
	{
		return std::nullopt;
	}
}

template<>
std::optional<float> Async::Field::As<float>() const noexcept
{
	try
	{
		return std::stof(m_data);
	}
	catch (...)
	{
		return std::nullopt;
	}
}

template<>
std::optional<std::string> Async::Field::As<std::string>() const noexcept
{
	return m_data;
}

template<>
std::optional<bool> Async::Field::As<bool>() const noexcept
{
	try
	{
		if (m_data == "true" || m_data == "TRUE" || m_data == "1")
			return true;

		if (m_data == "false" || m_data == "FALSE" || m_data == "0")
			return false;

		return std::nullopt;
	}
	catch (...)
	{
		return std::nullopt;
	}
}

Async::Field::Field(const char* apData)
	: m_data(apData)
{
}
