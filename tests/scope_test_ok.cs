int global_x = 100;
int result;

{
    int a = 10;
    int b = 20;
    
    result = a + b; 

    {
        int c = 30;

        result = a + c + global_x;
    }
    

    result = result + b;
}


result = global_x + 1;