#!/usr/bin/python
import sys
from latexwriter import *
from xmlToLatex import *

from htmlwriter import *
from xmlToHtml import *

from optparse import OptionParser

######  OPTIONS  ######
optionParser = OptionParser()
optionParser.add_option(
	"-i", "--input",
	type    = "string",
	dest    = "filename",
	action  = "append",
	help    = "generate Latex or HTML from XML QC report file",
	metavar = "FILE"
	)

optionParser.add_option(
	"-l", "--list",
	type    = "string",
	dest    = "fieldListFile",
	action  = "append",
	help    = "display all available fields",
	metavar = "FILE"
	)

optionParser.add_option(
	"-c", "--choice",
	type    = "string",
	dest    = "fieldsToRemove",
	action  = "append",
	help    = "field (or stream index) not to display into the output file",
	metavar = "FIELDS"
	)

optionParser.add_option(
	"-f", "--format",
	type    = "string",
	dest    = "format",
	action  = "append",
	help    = "output format (tex or html)",
	metavar = "FORMAT"
	)

(options, args) = optionParser.parse_args()
# print options
# print args
parser = XmlParser()

######  List of Fields  ######
if options.fieldListFile :
	for arg in args:
		options.fieldListFile.append( arg )

	for fieldListFile in options.fieldListFile :
		parser.parseXml( fieldListFile )
		if options.fieldsToRemove :
			for arg in args:
				options.fieldListFile.append( arg )
			for field in options.fieldsToRemove :
				parser.removeField( field )
		parser.displaySections()


######  Display Help  ######
if len( args ) == 0 and options.filename == None and options.fieldListFile == None:
	optionParser.print_help()
	sys.exit()


######  Generate Report  ######
if not options.filename:
	raise RuntimeError( "Input file required ( -i FILE )" )
for inputFile in options.filename :

	if not options.format :
		raise RuntimeError( "Output format required ( -f FORMAT )" )
	
	outputExt = ""
	for format in options.format :
		if format != "tex" and format != "html" :
			raise ValueError( "Unavailable output format. Choice : tex, html" )

		outputExt = format
		outputFile = inputFile.replace( "-merged", "" )
		outputFile = outputFile.replace( ".xml", "." + outputExt )
		print ">>> Output filename : " + outputFile

		parser.parseXml( inputFile, outputExt )
		if options.fieldsToRemove :
			for arg in args:
				options.fieldsfile.append( arg )
			for field in options.fieldsToRemove :
				parser.removeField( field )
		
		if outputExt == "tex" :
			xtl = XmlToLatex()
			xtl.convert( parser )
			stream = xtl.getLatexStream()
		else : 
			xth = XmlToHtml()
			xth.convertToFile( parser )
			stream = xth.getHtmlStream()

		file = open( outputFile, "w+" )
		file.write( stream )
		file.close()
