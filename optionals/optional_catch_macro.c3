fn void main(String[] args)
{
    int! optional_value = IoError.FILE_NOT_FOUND?;

    anyfault excuse = @catch(optional_value);
    if (excuse)
    {
        io::printfn("Excuse found: %s", excuse);
    }
}
