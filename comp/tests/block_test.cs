int global_a;
double global_b = 5.0;

{
    int x;
    double y = 10.5;

    x = 10;
    y = x + global_b;

    global_a += 1;
}

{}

{
    int outer_z = 100;

    {
        int inner_w = 200;
        outer_z = inner_w;

    }

    global_a = outer_z; 
}

global_a = 0;