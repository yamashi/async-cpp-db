#include "DbPool.hpp"
#include <thread>

std::future<std::optional<Async::Result>> Async::DbPool::Fetch(const std::string& aQuery) noexcept
{
	return std::async(std::launch::async, [=]() -> std::optional<Async::Result> {

		std::shared_ptr<Db> db;
		{
			std::scoped_lock _{ m_lock };

			if (!m_dbs.empty())
			{
				db = m_dbs.top();
				m_dbs.pop();
			}
		}

		if (!db)
			db = Connect();

		if (!db)
			return std::nullopt;

		auto result = db->Fetch(aQuery);

		// Only add it to the pool if the query was successful, this will cause malformed requests to lose connections but this is an edge case
		if(result)
		{
			std::scoped_lock _{ m_lock };
			m_dbs.push(db);
		}

		return result;
	});
}

std::future<std::optional<std::string>> Async::DbPool::Execute(const std::string& aQuery) noexcept
{
	return std::async(std::launch::async, [this, aQuery]() {
		return DoExecute(aQuery);
	});
}

void Async::DbPool::ExecuteAndForget(const std::string& aQuery) noexcept
{
	std::thread([this, aQuery](){ 
		DoExecute(aQuery);
	}).detach();
}

Async::DbPool::DbPool(std::string acHost, uint16_t aPort, std::string acUsername, std::string acPassword, std::string acDatabase, uint32_t aCount)
	: m_host(std::move(acHost))
	, m_port(aPort)
	, m_username(std::move(acUsername))
	, m_password(std::move(acPassword))
	, m_database(std::move(acDatabase))
{
	for (auto i = 0u; i < aCount; ++i)
	{
		auto db = Connect();
		if(db)
			m_dbs.push(db);
	}
}

std::optional<std::string> Async::DbPool::DoExecute(const std::string& aQuery) noexcept
{
	std::shared_ptr<Db> db;
	{
		std::scoped_lock _{ m_lock };

		if (!m_dbs.empty())
		{
			db = m_dbs.top();
			m_dbs.pop();
		}
	}

	if (!db)
		db = Connect();

	if (!db)
		return std::nullopt;

	auto result = db->Execute(aQuery);

	// Only add it to the pool if the query was successful, this will cause malformed requests to lose connections but this is an edge case
	if (!result)
	{
		std::scoped_lock _{ m_lock };
		m_dbs.push(db);
	}

	return result;
}

std::shared_ptr<Async::Db> Async::DbPool::Connect() noexcept
{
	auto db = std::make_shared<Async::Db>();
	auto res = db->Connect(m_host, m_port, m_username, m_password, m_database);
	if (res)
		return nullptr;

	return db;
}
