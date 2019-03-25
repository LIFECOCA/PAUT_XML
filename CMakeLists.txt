#设置cmake版本号
cmake_minimum_required(VERSION 2.8.11)

#设置工程名称
PROJECT(PAUT_XML)

#use SIMD and close exception
set(CMAKE_CXX_FLAGS "/arch:AVX /EHsc")

#新添加
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#main
set(PROJECT_MAIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" CACHE PATH "MAIN directory")
file(GLOB project_main_header_files ${PROJECT_MAIN_DIR}/*.h)
file(GLOB project_main_source_files ${PROJECT_MAIN_DIR}/*.cpp)
source_group("Header Files" FILES ${project_main_header_files})
source_group("Source Files" FILES ${project_main_source_files})


#XML
set(PROJECT_PAUT_XML_DIR "${PROJECT_MAIN_DIR}/XML" CACHE PATH "PAUT XML directory")
set(PROJECT_PAUT_XML_READER_DIR "${PROJECT_PAUT_XML_DIR}/Reader" CACHE PATH "PAUT XML Reader directory")
file(GLOB project_paut_xml_reader_header_files ${PROJECT_PAUT_XML_READER_DIR}/*.h)
file(GLOB project_paut_xml_reader_source_files ${PROJECT_PAUT_XML_READER_DIR}/*.cpp)
source_group("Header Files\\XML\\Reader" FILES ${project_paut_xml_reader_header_files})
source_group("Source Files\\XML\\Reader" FILES ${project_paut_xml_reader_source_files})
set(PROJECT_PAUT_XML_WRITER_DIR "${PROJECT_PAUT_XML_DIR}/Writer" CACHE PATH "PAUT XML Writer directory")
file(GLOB project_paut_xml_writer_header_files ${PROJECT_PAUT_XML_WRITER_DIR}/*.h)
file(GLOB project_paut_xml_writer_source_files ${PROJECT_PAUT_XML_WRITER_DIR}/*.cpp)
source_group("Header Files\\XML\\Writer" FILES ${project_paut_xml_writer_header_files})
source_group("Source Files\\XML\\Writer" FILES ${project_paut_xml_writer_source_files})

#New XML
set(PROJECT_PAUT_NEWXML_DIR "${PROJECT_MAIN_DIR}/NewXML" CACHE PATH "PAUT XML directory")
set(PROJECT_PAUT_NEWXML_READER_DIR "${PROJECT_PAUT_NEWXML_DIR}/Reader" CACHE PATH "PAUT XML Reader directory")
file(GLOB project_paut_newxml_reader_header_files ${PROJECT_PAUT_NEWXML_READER_DIR}/*.h)
file(GLOB project_paut_newxml_reader_source_files ${PROJECT_PAUT_NEWXML_READER_DIR}/*.cpp)
source_group("Header Files\\NEWXML\\Reader" FILES ${project_paut_newxml_reader_header_files})
source_group("Source Files\\NEWXML\\Reader" FILES ${project_paut_newxml_reader_source_files})
set(PROJECT_PAUT_NEWXML_WRITER_DIR "${PROJECT_PAUT_NEWXML_DIR}/Writer" CACHE PATH "PAUT XML Writer directory")
file(GLOB project_paut_newxml_writer_header_files ${PROJECT_PAUT_NEWXML_WRITER_DIR}/*.h)
file(GLOB project_paut_newxml_writer_source_files ${PROJECT_PAUT_NEWXML_WRITER_DIR}/*.cpp)
source_group("Header Files\\NEWXML\\Writer" FILES ${project_paut_newxml_writer_header_files})
source_group("Source Files\\NEWXML\\Writer" FILES ${project_paut_newxml_writer_source_files})

# Add the include directories for VTK and Qt 5 Widgets module to the compile lines.
include_directories(${PROJECT_MAIN_DIR}
					${PROJECT_PAUT_XML_DIR}
					${PROJECT_PAUT_XML_READER_DIR}
					${PROJECT_PAUT_XML_WRITER_DIR}
					${PROJECT_PAUT_NEWXML_READER_DIR}
					${PROJECT_PAUT_NEWXML_WRITER_DIR})

add_executable(${PROJECT_NAME}
			   ${project_main_header_files}
			   ${project_main_source_files}
			   ${project_paut_xml_reader_header_files}
			   ${project_paut_xml_reader_source_files}
			   ${project_paut_xml_writer_header_files}
			   ${project_paut_xml_writer_source_files}
			   ${project_paut_newxml_reader_header_files}
			   ${project_paut_newxml_reader_source_files}
			   ${project_paut_newxml_writer_header_files}
			   ${project_paut_newxml_writer_source_files})

#设定可执行二进制文件的目录
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin) 
