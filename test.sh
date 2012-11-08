#!/bin/bash

start()                          
{                               
if  pidof apache2 >/dev/null;
                   
then               
        echo apache2 active
else                 
        echo apache2 inactive
fi          
}           
             
case "$1" in 
start)       
        start                   
        ;;                      
*)                              
        echo "Usage: $0 {start}"
        exit 1                  
esac    
