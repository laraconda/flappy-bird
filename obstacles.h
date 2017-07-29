struct WIN_pos_size;

struct obstacle {
    char width;
    int y;
};

struct pair_of_obstacles {
    int x;
    struct obstacle obs_a;
    struct obstacle obs_b;		    
};

struct obstacles_settings {
    unsigned int width;
    int spacing;
    int min_vert_gap;
    int max_vert_diff_neighbors; // max vertical diff between neigh...
};

void print_obstacles(
    struct WIN_pos_size win, struct pair_of_obstacles *pairs,
    unsigned int, struct obstacles_settings sett);

void init_obstacles(
    struct WIN_pos_size win, struct pair_of_obstacles *pairs,
    unsigned int, struct obstacles_settings sett);

void advance_obstacles(
    struct WIN_pos_size win, struct pair_of_obstacles *pairs,
    unsigned int, struct obstacles_settings sett);
