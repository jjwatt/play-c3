import std::io;

fn void! test()
{
    // Return an Excuse by adding '?' after the fault.
    return IoError.FILE_NOT_FOUND?;
}

fn void main(String[] args)
{
    // If the Optional is empty, assign the
    // Excuse to a variable:
    if (catch excuse = test())
    {
        io::printfn("test() gave an Excuse: %s", excuse);
    }
}
