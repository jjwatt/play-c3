import std::io;

fn int test(int input, int input2)
{
    io::printn("test(): inside function body");
    return input;
}

fn void main(String[] args)
{
    int! first_optional = IoError.FILE_NOT_FOUND?;
    int! second_optional = 7;

    // Return first excuse we find
    int! third_optional = test(first_optional, second_optional);
    if (catch excuse = third_optional)
    {
        // excuse == IoError.FILE_NOT_FOUND
        io::printfn("third_optional's Excuse: %s", excuse);
    }
}
