#include "raycast.hpp"
#include <unistd.h>
#include <sys/types.h>

void	playAudio(std::string file)
{
	std::string command = "afplay " + file;;
	pid_t pid = fork();
	if (pid == 0)
		exit(system(command.c_str()));
}