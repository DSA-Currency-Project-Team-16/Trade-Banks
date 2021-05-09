This project, in addition to the header file and the .c files, has a list of 1000(maximum capacity) tradebanks in a file called tradebanks.txt    
and 200(maximum capcacity) currencies in currency.txt.   

Constraint: No. of characters name of the bank and currency should not exceed 49.   

Key for using the file:-    
Press 1 to add a Tradebank     
Press 2 to Add currencies for a given tradebank    
Press 3 to Get conversion rates for each tradebank(Add edge)   
Press 4 to delete a tradebank   
Press 5 to delete currency from a tradebank   
Press 6 to delete a currency conversion rate(edge)   
Press 7 to detect cycle in  graph   
Press 8 to find the second best path    
Press 9 to find the best path  
Press 10 to add edge weight manually  
Press 11 to exit the input programme  

If we press 1, for example, the program randomly selects a name from the list of tradebanks and inserts the tradebank.  
If we press 2, program prompts us to enter the name of a tradebank how many currencies to add. Then it randomly chooses x(the no entered)  
names from the list of currencies and adds them to the tradebank.  
if we press 3, program asks us to enter the tradebank and the currencies names and  then randomly generates the edgeweight and inserts the edge.  
If we press 4, the program prompts user to enter the name of tradebank and deletes it.  
If we press 5, the program prompts the user for the names of the tradebank and the currency and deletes the currency from the tradebank.  
If we press 6, the program asks user to enter the tradebank and the source and destination currency.  
On pressing 7, the program asks user to enter the tradebank name and checks if there is a cycle in the graph or not.  
Upon pressing 8, the program asks the user to input currencies A and B in that order. Then, it finds the second shortest path from A to B among all  
tradebanks.      
Upon pressing 9, the program asks the user to input the currencies A and B in that order. Then, it finds the shortest path from A to B among all  
graphs and prints the path.  
Upon pressing 10, the program prompts the user to enter edgeweight, tradebank, and the currencies and adds the edge to the graph.  
Upon pressing 11, the program exits.   
 
