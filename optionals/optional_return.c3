import std::io;

fn int test(int input)
{
    io::printn("test(): inside function body");
    return input;
}

fn void main(String[] args)
{
    int! optional_argument = 7;

    // `optional_argument` makes returned `returned_optional`
    // Optional too:
    int! returned_optional = test(optional_argument);

    io::printfn("returned_optional: %d", returned_optional)!!;
}
