enum color;

/*
color -> sets all future stdout text color to this
returns -> void
sets all future stdout text color to specified color
*/
void set_color(unsigned char color);

/*
color -> sets all future stderr text color to this
returns -> void
sets all future stderr text color to specified color
*/
void set_err_color(unsigned char color);

/*
returns -> void
prints introduction to eggsh
*/
void greeting();

/*
error -> string to print to stderr
returns -> void
sets color to red and prints to stderr
*/
void print_error(char* error);

/*
returns -> void
prints user directory, hostname, username, etc
*/
void print_user_info();
