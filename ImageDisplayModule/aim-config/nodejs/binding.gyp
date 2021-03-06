#
# @file binding.gyp
# @brief This file provides the configuration and building options for node-gyp
#
# This file is created at "DoBots". It is open-source software and part of "AI". 
# This software is published under the GNU General Lesser Public Licence license (GPLv3).
#
# Copyright © 2013 Bart van Vliet <bart@almende.org>
#
# @author                    Bart van Vliet
# @date                      nov 19, 2013
# @organisation              DoBots
# @project                   AI
#
{
	"targets": [
		{
			"target_name": "ImageDisplayModule",
			
			"include_dirs": [
				"../../inc",
				"../../aim-core/inc"
				
			],
			
			"dependencies":[
			],
			
			"cflags": [
			],
			
			"libraries": [
			],
			
			"ldflags": [
				"-pthread",
			],
			
			"sources":[
				"../../aim-core/src/ImageDisplayModule.cpp",
				"ImageDisplayModuleNode.cc",
				"../../src/ImageDisplayModuleExt.cpp"
			],
		}
	]
}
