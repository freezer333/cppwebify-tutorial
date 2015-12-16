
extern "C" {
    // the old main, renamed - with a third parameter
    // to direct output to a file as needed
    int generate_args(int argc, char *argv[], FILE * out);

    // an adapter function when the caller hasn't
    // received under through command line arguments
    int generate(int under, FILE *out);
}
