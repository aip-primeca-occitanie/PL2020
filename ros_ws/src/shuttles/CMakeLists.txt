cmake_minimum_required(VERSION 2.8.3)
project(shuttles)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs 
  genmsg
  message_generation
)

add_message_files(
	FILES
	msgShuttleChange.msg
	msgShuttleCreate.msg
	)

add_service_files(
	FILES
	srvGetShuttleStatus.srv
	srvGetEmptyShuttles.srv
	)

generate_messages( DEPENDENCIES std_msgs)
catkin_package( CATKIN_DEPENDS roscpp message_runtime) 


include_directories(${catkin_INCLUDE_DIRS})


add_executable(shuttlesManager src/mainCommandeShuttle.cpp src/shuttle.cpp src/shuttleManager.cpp)  
target_link_libraries(shuttlesManager ${catkin_LIBRARIES})
add_dependencies(shuttlesManager ${PROJECT_NAME}_gencpp ${${shuttles}_EXPORTED_TARGETS})
