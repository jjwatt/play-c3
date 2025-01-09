import std::io;

fn void main(String[] args)
{
    // Returns Optional with result of type `int` or an Excuse
    int! first_optional = 7;

    // This is Optional too:
    int! second_optional = first_optional + 1;
    // Needs !! (unsafe unwrap, iiuc)
    io::printfn("first_optional: %d", first_optional)!!;
    io::printfn("second_optional: %d", second_optional)!!;
}
