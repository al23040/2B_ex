int result;

{
    int a = 10;
    int b = 1;
    
    result = a; 

    {
        int a = 100;
        
        result = result + a;
        {
            int c = 29;
            result += c;
        }
    }
    
    result = result + a + b;
}