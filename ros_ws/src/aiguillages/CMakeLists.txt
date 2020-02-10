cmake_minimum_required(VERSION 2.8.3)
project(aiguillages)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  rospy
  std_msgs
)

add_message_files(FILES Capteurs.msg Actionneurs.msg ExchangeSh.msg Msg_Aiguillage.msg)

generate_messages(DEPENDENCIES std_msgs)
catkin_package(CATKIN_DEPENDS roscpp message_runtime)


include_directories(${catkin_INCLUDE_DIRS})
include_directories(include ${OpenCV_INCLUDE_DIRS})


add_executable(run_A11 src/aiguillage_11.cpp src/Aiguillage.cpp)
target_link_libraries(run_A11 ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
add_dependencies(run_A11 ${PROJECT_NAME}_gencpp ${catkin_EXPORTED_TARGETS})

add_executable(run_A12 src/aiguillage_12.cpp src/Aiguillage.cpp)
target_link_libraries(run_A12 ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
add_dependencies(run_A12 ${PROJECT_NAME}_gencpp ${catkin_EXPORTED_TARGETS})

add_executable(run_A1 src/aiguillage_1.cpp src/Aiguillage.cpp)
target_link_libraries(run_A1 ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
add_dependencies(run_A1 ${PROJECT_NAME}_gencpp ${catkin_EXPORTED_TARGETS})

add_executable(run_A2 src/aiguillage_2.cpp src/Aiguillage.cpp)
target_link_libraries(run_A2 ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
add_dependencies(run_A2 ${PROJECT_NAME}_gencpp ${catkin_EXPORTED_TARGETS})

add_executable(run_A3 src/aiguillage_3.cpp src/Aiguillage.cpp)
target_link_libraries(run_A3 ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
add_dependencies(run_A3 ${PROJECT_NAME}_gencpp ${catkin_EXPORTED_TARGETS})

add_executable(run_A10 src/aiguillage_10.cpp src/Aiguillage.cpp)
target_link_libraries(run_A10 ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
add_dependencies(run_A10 ${PROJECT_NAME}_gencpp ${catkin_EXPORTED_TARGETS})
