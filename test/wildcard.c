#define ZD_IMPLEMENTATION
#define ZD_WILDCARD
#define ZD_TEST
#include "zd.h"

char *test1(void)
{
    /*** Test cases for * wildcard matching 0 or more characters ***/
    zd_assert(zd_wildcard_match("abc", "a*c") == true, NULL);  
    zd_assert(zd_wildcard_match("abc", "a*") == true, NULL);    
    zd_assert(zd_wildcard_match("abc", "*") == true, NULL);     
    zd_assert(zd_wildcard_match("abc", "*bc") == true, NULL);   
    zd_assert(zd_wildcard_match("abc", "a*bc") == true, NULL);  
    zd_assert(zd_wildcard_match("abc", "*a*bc*") == true, NULL); 

    /*** Test cases for ? wildcard matching any single character ***/
    zd_assert(zd_wildcard_match("abc", "a?c") == true, NULL);   
    zd_assert(zd_wildcard_match("abc", "a?") == false, NULL);    
    zd_assert(zd_wildcard_match("abc", "a?b") == false, NULL);  
    zd_assert(zd_wildcard_match("abc", "??c") == true, NULL);   
    zd_assert(zd_wildcard_match("abc", "??d") == false, NULL);  

    /*** Test cases for combinations of * and ? ***/
    zd_assert(zd_wildcard_match("abc", "a*?c") == true, NULL);  
    zd_assert(zd_wildcard_match("abc", "a*bc") == true, NULL);  
    zd_assert(zd_wildcard_match("abc", "a?b*") == false, NULL);  
    zd_assert(zd_wildcard_match("abc", "a?*c") == true, NULL);  
    zd_assert(zd_wildcard_match("abc", "a?*d") == false, NULL); 
    zd_assert(zd_wildcard_match("abc", "*?c") == true, NULL);   
    zd_assert(zd_wildcard_match("abc", "a*?*c") == true, NULL); 

    /*** Test cases for * matching start of string ***/
    zd_assert(zd_wildcard_match("abc", "*bc") == true, NULL);  
    zd_assert(zd_wildcard_match("abc", "*b?") == true, NULL);   
    zd_assert(zd_wildcard_match("abc", "*a?") == false, NULL);  
    zd_assert(zd_wildcard_match("abc", "*a?b") == false, NULL); 
    zd_assert(zd_wildcard_match("abcdef", "*?*?*?*") == true, NULL); 

    /*** Test cases for empty string ***/
    zd_assert(zd_wildcard_match("", "") == true, NULL);          
    zd_assert(zd_wildcard_match("", "*") == true, NULL);         
    zd_assert(zd_wildcard_match("", "a*") == false, NULL);       
    zd_assert(zd_wildcard_match("", "?") == false, NULL);        
    zd_assert(zd_wildcard_match("", "*?") == false, NULL);       

    /*** Test cases for string length comparison ***/
    zd_assert(zd_wildcard_match("abc", "abcd") == false, NULL);   
    zd_assert(zd_wildcard_match("abcd", "abc") == false, NULL);   
    zd_assert(zd_wildcard_match("abc", "ab*c") == true, NULL);     
    zd_assert(zd_wildcard_match("a", "a*") == true, NULL);         
    zd_assert(zd_wildcard_match("a", "*") == true, NULL);          
    zd_assert(zd_wildcard_match("a", "a") == true, NULL);          

    /*** Test cases for multiple * and ? wildcard combinations ***/
    zd_assert(zd_wildcard_match("abcdef", "a*def") == true, NULL); 
    zd_assert(zd_wildcard_match("abcdef", "a*b?d*") == true, NULL); 
    zd_assert(zd_wildcard_match("abcdef", "a?c*d*") == true, NULL); 
    zd_assert(zd_wildcard_match("abcdef", "a*d?f") == true, NULL); 
    zd_assert(zd_wildcard_match("abcdef", "*?d*?*") == true, NULL); 

    return "test zd_wildcard done!";
}

int main(void)
{
    struct zd_testsuite suite = {0};
    suite.name = "test zd_regex";

    zd_run_test(&suite, test1);
    zd_test_summary(&suite);

    return 0;
}
