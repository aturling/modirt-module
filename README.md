# MO DIRT (Missourians Doing Impact Research Together) Module

## Introduction

The MO DIRT module extends the DIRT Citizen Science and DIRT DataTable modules 
with customizations specific to the MO DIRT project. In particular, this module 
adds a new content type that stores lab results and includes a form that allows 
site admins to upload spreadsheets to populate lab results for multiple 
collection sites. It also makes small location-specific changes to the basic 
DIRT module, e.g., sets the default site state to Missouri, sets the data 
search portal map center to the center of Missouri, etc.


## Uses

* Collecting citizen science survey data in Missouri
This module customizes the core DIRT module to set the collection location to 
the state of Missouri.

* Collecting soil lab data
A new survey type is added to store the results of soil lab analysis variables. 
New variable filters and column groups in the data search portal table are 
added for the lab data.

* Updating select site description fields
The site description survey is modified to allow some fields to be updated from 
time to time as needed, e.g., present and past land management.


## Requirements

The MO DIRT module requires DIRT and DIRT DataTable, along with the following 
contributed modules:

* Honeypot (https://www.drupal.org/project/honeypot)
* Views (https://www.drupal.org/project/views)

Additionally, the following library is required:

* PHPExcel (https://github.com/PHPOffice/PHPExcel)


## Configuration

After installation, navigate to admin/config/dirt to access the configuration
pages. Most of these pages are packaged with the DIRT and DIRT DataTable 
modules, but there is one new configuration option at 
admin/config/dirt/surveys/upload which allows site admins to upload Excel 
(or CSV-formatted) spreadsheets containing lab results for multiple 
collection sites.
