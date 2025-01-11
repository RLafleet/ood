#ifndef COW_H
#define COW_H

#include <cassert>
#include <memory>

template<typename Value>
class CoW
{
	struct WriteProxy
	{
		explicit WriteProxy(Value* value) noexcept
			: m_value_ptr{ value }
		{
		}

		WriteProxy(const WriteProxy&) = delete;

		WriteProxy& operator=(const WriteProxy&) = delete;

		Value& operator*() const& = delete;

		[[nodiscard]] Value& operator*() const&& noexcept
		{
			return *m_value_ptr;
		}

		Value* operator->() const& = delete;

		Value* operator->() const&& noexcept
		{
			return m_value_ptr;
		}

	private:
		Value* m_value_ptr;
	};

public:
	CoW()
		: m_value(std::make_shared<Value>())
	{
	}

	explicit CoW(Value&& value)
		: m_value(std::make_shared<Value>(std::move(value)))
	{
	}

	explicit CoW(const Value& value)
		: m_value(std::make_shared<Value>(value))
	{
	}

	const Value& operator*() const noexcept
	{
		assert(m_value);
		return *m_value;
	}

	const Value* operator->() const noexcept
	{
		assert(m_value);
		return m_value.get();
	}

	template<typename ModifierFn>
	void Write(ModifierFn&& modify)
	{
		EnsureUnique();

		std::forward<ModifierFn>(modify)(*m_value);
	}

	WriteProxy Write() && = delete;

	[[nodiscard]] WriteProxy Write()&
	{
		EnsureUnique();

		return WriteProxy(m_value.get());
	}

	Value& WriteBad()
	{
		EnsureUnique();

		return *m_value;
	}

private:
	void EnsureUnique()
	{
		assert(m_value);

		if (m_value.use_count() > 1)
		{
			m_value = std::make_share	d<Value>(*m_value);
		}
	}

	std::shared_ptr<Value> m_value;
};

#endif //COW_H