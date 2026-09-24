#include "Command Definition.h"

namespace Command::random
{
	namespace
	{

	}

	bool cmd::setArg(const std::string& argName, const::Command::argument& arg)
	{
		auto argDef = definition->argDefinitions.find(argName);

		if (argDef == definition->argDefinitions.end())
			return false;

		if (!Argument::checkArgtypeMatch(arg, argDef->second.type))
			return false;

		args.at(argName) = arg;
		return true;

	}

	cmd::cmd(const dfn* def)
	{
		definition = def;
		for (const auto& [argName, argMD] : def->argDefinitions)
			args[argName] = argMD.defaultValue;
	}

}
