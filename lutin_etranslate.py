#!/usr/bin/python
import lutin.debug as debug
import lutin.tools as tools


def get_type():
	return "LIBRARY"

def get_desc():
	return "E-translate: Ewol runtime translate tool"

def get_licence():
	return "APACHE-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return "authors.txt"

def get_version():
	return "version.txt"

def configure(target, my_module):
	# add extra compilation flags:
	my_module.add_extra_flags()
	# add the file to compile:
	my_module.add_src_file([
	    'etranslate/etranslate.cpp',
	    'etranslate/debug.cpp',
	    ])
	my_module.add_header_file([
	    'etranslate/etranslate.hpp',
	    ])
	
	# name of the dependency
	my_module.add_depend([
	    'elog',
	    'etk',
	    'ejson',
	    ])
	
	my_module.add_path(".")
	
	return True

