import std::io;

// Returns Optional result with `int` type or empty with an Excuse
fn int! reliable_function()
{
    return 7; // Return a result
}

fn void main(String[] args)
{
    int! reliable_result = reliable_function();

    // Unwrap the result from reliable_result
    if (try reliable_result)
    {
        // reliable_result is unwrapped in this scope, can be used as normal
        io::printfn("reliable_result: %s", reliable_result);
    }
}
