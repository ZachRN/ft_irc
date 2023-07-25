// #include "server.hpp"
// #include "client.hpp"
// #include <iostream>

// void	user_not_found()
// {
// 	Server server("pass", 123);
// 	std::cout << "USERS_NOT_FOUND_TESTS\n--------------" << std::endl;
// 	if (server.get_client("Zach") == nullptr)
// 		std::cout << "Client not found" << std::endl;
// 	std::cout << "-------------" << std::endl;
// }
// void	add_duplicate_user()
// {
// 	Server server("pass", 123);
// 	std::cout << "ADD_DUPLICATE_USER_TESTS\n--------------" << std::endl;
// 	if (server.add_client("Zach", "Zachary") != SUCCESS)
// 		std::cout << "Failed to Add Client to list." << std::endl;
// 	if (server.add_client("Zach", "Doesn't Matter") == NAME_ALREADY_INUSE)
// 		std::cout << "Couldn't Add Client Name already in use" << std::endl;
// 	std::cout << "-------------" << std::endl;
// }
// void	name_syntax()
// {
// 	Server server("pass", 123);
// 	std::cout << "NAME_SYNTAX_TESTS\n--------------" << std::endl;
// 	if (server.add_client("Za ch", "Zachary") != SUCCESS)
// 		std::cout << "Failed to add, syntax invalid" << std::endl;
// 	if (server.add_client("Za.ch", "Zachary") != SUCCESS)
// 		std::cout << "Failed to add, syntax invalid (invalid nickname)" << std::endl;
// 	if (server.add_client("Za-ch", "Zach.ry") != SUCCESS)
// 		std::cout << "Failed to add, syntax invalid (invalid username)" << std::endl;
// 	if (server.add_client("Za_ch", "ZacharyTHisisTooLong") != SUCCESS)
// 		std::cout << "Failed to add, syntax invalid (too long)" << std::endl;
// 	if (server.add_client("Zaaaach", "") != SUCCESS)
// 		std::cout << "Failed to add, syntax invalid (name too short)" << std::endl;
// 	std::cout << "-------------" << std::endl;
// }
// void	remove_invalid_user()
// {
// 	Server server("pass", 123);
// 	std::cout << "REMOVE_INVALID_USER_TESTS\n--------------" << std::endl;
// 	if (server.remove_client("Zach") == NO_CLIENT_FOUND)
// 		std::cout << "No Client Found to Remove" << std::endl;
// 	std::cout << "-------------" << std::endl;
// }

// void	run_tests(void)
// {
// 	user_not_found();
// 	add_duplicate_user();
// 	name_syntax();
// 	remove_invalid_user();
// }
//THIS ALL NEEDS BE REWRITTEN