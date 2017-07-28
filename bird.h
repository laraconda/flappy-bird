struct bird {
    int x;
    int y;
    float acc;
};


char *get_bird_repr(void);
void print_bird(struct bird *b);
void move_bird(struct bird *b);
int get_bird_size(void);

