/**** Maxime Maurin - Projet Long N7 2017 ****/

#include <ros/ros.h>
#include "product.h"


// Constructeur
Product::Product(std::string productName, int productFirstDestination, int initProductColor,int productManRangeSize, int initProductNumber)
{
	name = productName;
	firstDestination = productFirstDestination;
	productColor = initProductColor;
	manRangeSize = productManRangeSize;
	productNumber = initProductNumber;
}

// Destructeur
Product::~Product(){}


