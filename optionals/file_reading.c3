import std::io;

<*
 Function modifies 'buffer'
 Returns an Optional with a 'char[]' result
 OR an empty Optional with an Excuse
*>
fn char[]! read_file(String filename, char[] buffer)
{
    // Return Excuse if opening a file failed, using Rethrow `!`
    File file = file::open(filename, "r")!;

    // At scope exit, close the file.
    // Discard the Excuse from file.close() with (void) cast
    defer (void)file.close();

    // Return Excuse if reading failed, using Rethrow `!`
    file.read(buffer)!;
    return buffer; // return a buffer result
}

fn void! test_read()
{
    char[] buffer = mem::new_array(char, 100);
    defer free(buffer); // Free memory on scope exit

    char[]! read_buffer = read_file("file_to_open.txt", buffer);
    // Catch the empty Optional and assign the Excuse
    // to `excuse`
    if (catch excuse = read_buffer)
    {
        io::printfn("Excuse found: %s", excuse);
        // Returning Excuse using the `?` suffix
        return excuse?;
    }

    // `read_buffer` behaves like a normal variable here
    // because the Optional being empty was handled by 'if (catch)'
    // which automatically unwrapped 'read_buffer' at this point.
    io::printfn("read_buffer: %s", read_buffer);
}

fn void main()
{
    test_read()!!; // Panic on failure.
}
