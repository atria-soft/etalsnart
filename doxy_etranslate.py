#!/usr/bin/python
import os
import doxy.module as module
import doxy.debug as debug
import doxy.tools as tools

def create(target, module_name):
	my_module = module.Module(__file__, module_name)
	my_module.set_version("version.txt")
	my_module.set_title("E-translate: Ewol runtime translate tool")
	my_module.set_website("http://atria-soft.github.io/" + module_name)
	my_module.set_website_sources("http://github.com/atria-soft/" + module_name)
	my_module.add_path([
	    module_name,
	    "doc"
	    ])
	"""
	my_module.add_sample_path([
	    "sample"
	    ])
	"""
	my_module.add_depend([
	    'elog',
	    'etk',
	    'ejson',
	    ])
	my_module.add_exclude_symbols([
	    '*operator<<*',
	    ])
	my_module.add_exclude_file([
	    'debug.hpp',
	    ])
	my_module.add_file_patterns([
	    '*.hpp',
	    '*.md',
	    '*.py',
	    ])
	return my_module

