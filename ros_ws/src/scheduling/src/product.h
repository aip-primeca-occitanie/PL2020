/**** Maxime Maurin - Projet Long N7 2017 ****/

#ifndef PRODUCT
#define PRODUCT

#include <ros/ros.h>
#include <string.h>


class Product
{
public:
	
	std::string name;
	int firstDestination;
	int productColor;
	int manRangeSize;
	int productNumber;

	Product(std::string productName, int productFirstDestination, int initProductColor,int productManRangeSize, int initProductNumber);
	~Product();

};



#endif
