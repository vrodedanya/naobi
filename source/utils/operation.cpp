#include <naobi/utils/operation.hpp>

naobi::operation::operation(std::string operatorName, int priority, naobi::command::names commandAnalogue, naobi::operation::implementations implement) :
	_operator(std::move(operatorName)),
	_priority(priority),
	_commandAnalogue(commandAnalogue),
	_implementations(std::move(implement))
{

}

const std::string &naobi::operation::getOperator() const
{
	return _operator;
}

void naobi::operation::setOperator(const std::string &operatorName)
{
	_operator = operatorName;
}

int naobi::operation::getPriority() const
{
	return _priority;
}

void naobi::operation::setPriority(int priority)
{
	_priority = priority;
}

std::function<naobi::variable::sptr(naobi::variable::sptr, naobi::variable::sptr)>
naobi::operation::call(naobi::utils::type::names first, naobi::utils::type::names second)
{
	auto it = _implementations.find(std::make_pair(first, second));
	if (it == _implementations.end()) return nullptr;
	else return it->second;
}

naobi::command::names naobi::operation::getCommandAnalogue() const
{
	return _commandAnalogue;
}

void naobi::operation::setCommandAnalogue(naobi::command::names commandAnalogue)
{
	_commandAnalogue = commandAnalogue;
}

bool naobi::operation::operator<(const naobi::operation &rhs) const
{
	return _priority > rhs._priority;
}

bool naobi::operation::operator>(const naobi::operation &rhs) const
{
	return _priority < rhs._priority;
}

bool naobi::operation::operator<=(const naobi::operation &rhs) const
{
	return _priority >= rhs._priority;
}

bool naobi::operation::operator>=(const naobi::operation &rhs) const
{
	return _priority <= rhs._priority;
}

naobi::operation::sptr naobi::operation_manager::get(const std::string &operation)
{
	auto it = std::find_if(_operations.begin(), _operations.end(), [operation](const operation::sptr& s){return s->getOperator() == operation;});
	return it == _operations.end() ? nullptr : *it;
}

std::vector<naobi::operation::sptr> naobi::operation_manager::_operations = {
		std::make_shared<naobi::operation>("+", 2, naobi::command::names::ADD, naobi::operation::implementations({
			{std::make_pair(utils::type::names::INTEGER, utils::type::names::INTEGER),
			[](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			{
				auto result = std::make_shared<naobi::variable>("temp", utils::type::names::INTEGER);
				result->value() = std::get<int>(first->value()) + std::get<int>(second->value());
				return result;
			}},
			{std::make_pair(utils::type::names::FLOAT, utils::type::names::FLOAT),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::FLOAT);
				 result->value() = std::get<double>(first->value()) + std::get<double>(second->value());
				 return result;
			 }},
			{std::make_pair(utils::type::names::STRING, utils::type::names::STRING),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::STRING);
				 result->value() = std::get<std::string>(first->value()) + std::get<std::string>(second->value());
				 return result;
			 }},
			{std::make_pair(utils::type::names::BOOLEAN, utils::type::names::BOOLEAN),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = static_cast<bool>(std::get<bool>(first->value()) + std::get<bool>(second->value()));
				 return result;
			 }},
		})),
		std::make_shared<naobi::operation>("-", 2, naobi::command::names::SUB, naobi::operation::implementations({
			{std::make_pair(utils::type::names::INTEGER, utils::type::names::INTEGER),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::INTEGER);
				 result->value() = std::get<int>(first->value()) - std::get<int>(second->value());
				 return result;
			 }},
			{std::make_pair(utils::type::names::FLOAT, utils::type::names::FLOAT),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::FLOAT);
				 result->value() = std::get<double>(first->value()) - std::get<double>(second->value());
				 return result;
			 }},
			{std::make_pair(utils::type::names::STRING, utils::type::names::STRING),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 const std::string& var2 = std::get<std::string>(second->value());
				 std::size_t enter;
				 std::size_t length = var2.size();
				 std::string a = std::get<std::string>(first->value());
				 while ((enter = a.find(var2)) != std::string::npos)
				 {
					 a = a.erase(enter, length);
				 }
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::STRING);
				 result->value() = a;
				 return result;
			 }},
		})),
		std::make_shared<naobi::operation>("*", 1, naobi::command::names::MUL, naobi::operation::implementations({
			 {std::make_pair(utils::type::names::INTEGER, utils::type::names::INTEGER),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::INTEGER);
				 result->value() = std::get<int>(first->value()) * std::get<int>(second->value());
				 return result;
			 }},
			 {std::make_pair(utils::type::names::FLOAT, utils::type::names::FLOAT),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::FLOAT);
				 result->value() = std::get<double>(first->value()) * std::get<double>(second->value());
				 return result;
			 }},
			 {std::make_pair(utils::type::names::STRING, utils::type::names::INTEGER),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::STRING);
				 std::string temp;
				 for (int i = 0 ; i < std::get<int>(second->value()) ; i++)
				 {
					 temp += std::get<std::string>(first->value());
				 }
				 result->value() = temp;
				 return result;
			 }},
			 {std::make_pair(utils::type::names::BOOLEAN, utils::type::names::BOOLEAN),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<bool>(first->value()) && std::get<bool>(second->value());
				 return result;
			 }},
	 })),
		std::make_shared<naobi::operation>("/", 2, naobi::command::names::DIV, naobi::operation::implementations({
			 {std::make_pair(utils::type::names::INTEGER, utils::type::names::INTEGER),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::INTEGER);
				 result->value() = std::get<int>(first->value()) / std::get<int>(second->value());
				 return result;
			 }},
			 {std::make_pair(utils::type::names::FLOAT, utils::type::names::FLOAT),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::FLOAT);
				 result->value() = std::get<double>(first->value()) / std::get<double>(second->value());
				 return result;
			 }}
	 })),
		std::make_shared<naobi::operation>("==", 3, naobi::command::names::EQ, naobi::operation::implementations({
			 {std::make_pair(utils::type::names::INTEGER, utils::type::names::INTEGER),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<int>(first->value()) == std::get<int>(second->value());
				 return result;
			 }},
			 {std::make_pair(utils::type::names::FLOAT, utils::type::names::FLOAT),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<double>(first->value()) == std::get<double>(second->value());
				 return result;
			 }},
			 {std::make_pair(utils::type::names::STRING, utils::type::names::STRING),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<std::string>(first->value()) == std::get<std::string>(second->value());
				 return result;
			 }},
			 {std::make_pair(utils::type::names::BOOLEAN, utils::type::names::BOOLEAN),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<bool>(first->value()) == std::get<bool>(second->value());
				 return result;
			 }},
	 })),
		std::make_shared<naobi::operation>("!=", 3, naobi::command::names::NOT_EQ, naobi::operation::implementations({
			 {std::make_pair(utils::type::names::INTEGER, utils::type::names::INTEGER),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<int>(first->value()) != std::get<int>(second->value());
				 return result;
			 }},
			 {std::make_pair(utils::type::names::FLOAT, utils::type::names::FLOAT),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<double>(first->value()) != std::get<double>(second->value());
				 return result;
			 }},
			 {std::make_pair(utils::type::names::STRING, utils::type::names::STRING),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<std::string>(first->value()) != std::get<std::string>(second->value());
				 return result;
			 }},
			 {std::make_pair(utils::type::names::BOOLEAN, utils::type::names::BOOLEAN),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<bool>(first->value()) != std::get<bool>(second->value());
				 return result;
			 }},
	 })),
		std::make_shared<naobi::operation>(">=", 3, naobi::command::names::GREATER_OR_EQ, naobi::operation::implementations({
			 {std::make_pair(utils::type::names::INTEGER, utils::type::names::INTEGER),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<int>(first->value()) >= std::get<int>(second->value());
				 return result;
			 }},
			 {std::make_pair(utils::type::names::FLOAT, utils::type::names::FLOAT),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<double>(first->value()) >= std::get<double>(second->value());
				 return result;
			 }},
			 {std::make_pair(utils::type::names::STRING, utils::type::names::STRING),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<std::string>(first->value()) >= std::get<std::string>(second->value());
				 return result;
			 }},
			 {std::make_pair(utils::type::names::BOOLEAN, utils::type::names::BOOLEAN),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<bool>(first->value()) >= std::get<bool>(second->value());
				 return result;
			 }},
		})),
		std::make_shared<naobi::operation>("<=", 3, naobi::command::names::LESS_OR_EQ, naobi::operation::implementations({
			 {std::make_pair(utils::type::names::INTEGER, utils::type::names::INTEGER),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<int>(first->value()) <= std::get<int>(second->value());
				 return result;
			 }},
			 {std::make_pair(utils::type::names::FLOAT, utils::type::names::FLOAT),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<double>(first->value()) <= std::get<double>(second->value());
				 return result;
			 }},
			 {std::make_pair(utils::type::names::STRING, utils::type::names::STRING),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<std::string>(first->value()) <= std::get<std::string>(second->value());
				 return result;
			 }},
			 {std::make_pair(utils::type::names::BOOLEAN, utils::type::names::BOOLEAN),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<bool>(first->value()) <= std::get<bool>(second->value());
				 return result;
			 }},
	 })),
		std::make_shared<naobi::operation>(">", 3, naobi::command::names::GREATER, naobi::operation::implementations({
			 {std::make_pair(utils::type::names::INTEGER, utils::type::names::INTEGER),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<int>(first->value()) > std::get<int>(second->value());
				 return result;
			 }},
			 {std::make_pair(utils::type::names::FLOAT, utils::type::names::FLOAT),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<double>(first->value()) > std::get<double>(second->value());
				 return result;
			 }},
			 {std::make_pair(utils::type::names::STRING, utils::type::names::STRING),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<std::string>(first->value()) > std::get<std::string>(second->value());
				 return result;
			 }},
			 {std::make_pair(utils::type::names::BOOLEAN, utils::type::names::BOOLEAN),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<bool>(first->value()) > std::get<bool>(second->value());
				 return result;
			 }},
	 })),
		std::make_shared<naobi::operation>("<", 3, naobi::command::names::LESS, naobi::operation::implementations({
			 {std::make_pair(utils::type::names::INTEGER, utils::type::names::INTEGER),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<int>(first->value()) < std::get<int>(second->value());
				 return result;
			 }},
			 {std::make_pair(utils::type::names::FLOAT, utils::type::names::FLOAT),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<double>(first->value()) < std::get<double>(second->value());
				 return result;
			 }},
			 {std::make_pair(utils::type::names::STRING, utils::type::names::STRING),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<std::string>(first->value()) < std::get<std::string>(second->value());
				 return result;
			 }},
			 {std::make_pair(utils::type::names::BOOLEAN, utils::type::names::BOOLEAN),
			 [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			 {
				 auto result = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
				 result->value() = std::get<bool>(first->value()) < std::get<bool>(second->value());
				 return result;
			 }},
	 })),
		std::make_shared<naobi::operation>("%", 1, naobi::command::names::MOD, naobi::operation::implementations({
			{std::make_pair(utils::type::names::INTEGER, utils::type::names::INTEGER),
			  [](const naobi::variable::sptr& first, const naobi::variable::sptr& second) -> naobi::variable::sptr
			  {
				  auto result = std::make_shared<naobi::variable>("temp", utils::type::names::INTEGER);
				  result->value() = std::get<int>(first->value()) % std::get<int>(second->value());
				  return result;
			  }},
			})),
};