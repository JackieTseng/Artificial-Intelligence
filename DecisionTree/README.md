Project - Decision Tree
==============================


 Basic Function
--------------------

 1. This program will split the data into 10 pieces(i.train and i.test each piece).
 2. For each piece of data:
        perform 10-fold cross-validation and calculat the average correct data
 3. The script test.sh will do work in 1 and 2 based on different threshold value.


 How To Run
---------------------

 Before run the script test.sh, pleas compile the project

```sh
    make
    ./test.sh
```
 
 Clean the intermediate file by:
    
```sh
    make clean
```


 Extra File Provided(in picture)
----------------------
    - all.txt & average.txt: data about threshold and correct rate.
    - all.png & average.png: graph drawn according to data about.
    - bad-attr-to-choose.png & better-attr-to-choose.png: A intuitive picture of entropy after grouping according to certain attribution.
    
