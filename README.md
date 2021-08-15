# RIFFparser

Read, manipulate, and write riff files in c++.

I think I did this right but I also don't really know what I'm doing. A general example for creating and writing RIFF files using the `RIFF_t` class is in the main.cpp file.

## Creating a RIFF_t object

To create an empty RIFF file:

```cpp
RIFF_t my_riff;
```

To parse a RIFF file:

```cpp
RIFF_t my_riff("path/to/file.riff");
```

## Manipulating a RIFF_t object

### Getting chunks

A RIFF file has two basic chunk types. Data chunks are represented by the class `RIFF_chunk_data_t` . List chunks are represented by the class `RIFF_chunk_list_t` . The root chunk is a list type but has 'RIFF' as its chunk identifier ('LIST' for normal list chunks). To access the root chunk, and then its subchunks:

```cpp
auto root_chunk = my_riff.get_root_chunk();
auto sub_chunks = root_chunks.get_subchunks();
```

Subchunks are held in `std::vector<unique_ptr<RIFF_chunk_t>>` . Therefore, its a little clunky. Grabbing one specific chunk can get a little annoying:

```cpp
auto first_data_chunk = my_riff.get_root_chunk().get_subchunks()[0].get();
```

Alternatively the `get_chunk_with_id()` method can be used:

```cpp
auto wave_chunk = my_riff.get_chunk_with_id("WAVE");
```

This method returns a pointer to the first chunk that matches the given identifier. If there are multiple chunks with the same name (common for 'LIST' chunks), you'll have to loop through them manually.

### Adding chunks

Chunks are stored in `std::vector` objects. Adding new chunks is fairly straight forward, but annoying:

```cpp
my_riff.get_root_chunk().get_subchunks().push_back(std::make_unique<RIFF_chunk_data_t>("DATA", {0, 1, 2, 3, 4})); // for data chunks
my_riff.get_root_chunk().get_subchunks().push_back(std::make_unique<RIFF_chunk_list_t>("FORM")); // for list chunks
```

The constructor for data chunks allows for specifying both the identifier as well as a list of data to be added to it:

```cpp
RIFF_chunk_data_t(const char *id, const std::vector<uint8_t> &data = {});
```

The constructor for list chunks allows for specifying the form type:

```cpp
RIFF_chunk_list_t(const char *form_type);
```

Default constructors also exist. The identifier, form type, and data can be set later.

### Editing chunks

List chunks are edited using the `std::vector` interface. Data chunks have a method `get_data()` as well as `set_data()` for grabbing or replacing the data they hold:

```cpp
auto data = my_riff.get_root_chunk().get_subchunks()[0].get_data(); // get the data held in the first data chunk
my_riff.get_root_chunk().get_subchunks()[1].set_data(data); // replace the data in the second data chunk with the data from the first
```

Header data can also be edited. The `set_identifier()` method exists for both `RIFF_chunk_data_t` and `RIFF_chunk_list_t` . Generally, it should not be used for list chunks because the RIFF specification says list chunks should have 'LIST' as their identifier. The root chunk identifier can be also be changed, but really shouldn't be. The identifier 'RIFF' identifies the entire file as a RIFF file. The `RIFF_t` class will not parse files that do not begin with the 'RIFF' identifier. The `set_form_type()` method only exists for `RIFF_chunk_list_t` (data chunks do not have a form type). Both `set_identifier()` and `set_form_type()` require a four character `const char *` argument. Anything different from this will throw an exception:

```cpp
my_riff.get_root_chunk().set_identifier("RIFF"); // good
my_riff.get_root_chunk().set_form_type("a"); // bad
my_riff.get_root_chunk().set_form_type("fmt "); // general practice is to add spaces if the identifier is less than four characters long
```

## Writing a RIFF_t object

When using the file constructor, the file path is automatically added. Otherwise a new file path must be specified using the `set_filepath()` method:

```cpp
my_riff.set_filepath("new/path/to/file.riff");
```

Attempting to write a file without a set file path will throw an exception.
