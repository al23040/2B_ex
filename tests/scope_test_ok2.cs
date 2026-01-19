int result;

{
    int a = 10;
    int b = 1;
    
    result = a; 

    {
        int a = 100;
        
        result = result + a;
    }
    
    result = result + a + b;
}