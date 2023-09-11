struct WIN_pos_size;

/*
 * Obstacle structure.
 * If the obstacle is a top obstacle, the y represents where it ends.
 * If is a bottom one, the y represents where it starts.
 */
struct obstacle {
    char width;
    int y;
};

/*
 * Represents a top and botton pair of obstacles. Both on the same x 
 * coordinate.
 */
struct pair_of_obstacles {
    int x;
    struct obstacle top_obstacle;
    struct obstacle bottom_obstacle;		    
};

struct obstacles_settings {
    unsigned int width;
    int spacing;
    int min_vert_gap;
    /* Max vertical diff between neighbors.
     * If the difference is too big between two adjacent obstacles,
     * the bird may not be able to reach the safe gap of the second
     * obstacle.
     */
    int max_vert_diff_neighbors; 
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
