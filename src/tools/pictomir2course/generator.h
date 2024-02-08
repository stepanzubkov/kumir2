#ifndef GENERATOR_H
#define GENERATOR_H

#include "pictomir_data.h"

namespace pictomir2course
{

class generator
{
public:
	static void create_course(
		const game_t &game,
		const std::string &filename,
		const std::string &resources_path
	);

private:

	static void create_resource_files(
		const game_t &game,
		const std::string &resources_path
	);

	static std::string create_env_data(const environment_t &environment);


	static std::string create_kum_data(const program_t &program);


	static std::string create_kum_algorithm(
		const algorithm_t &algorithm,
		const size_t algorithm_index
	);

	static std::string create_testing_algorithm();


	static std::string create_task(
		const task_t &task,
		const size_t task_index,
		const std::string &resources_path
	);


	static std::string create_desc_data(
		const task_t &task,
		const size_t task_index
	);

};

}

#endif
