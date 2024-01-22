<h1>Cproject2i - C project for ESGI</h1><br>

<p>
    This project is a small 3D game made with C and OpenGL.<br>
    This was developped on MSVC compiler of Visual Studio 2022 and not tested on any other environment.
</p>
<br>
<h2>
    clone this repos with :
</h2>

```bash
git clone --recurse-submodules https://github.com/nicolas92g/Cproject2i
```

<h3>pull new Renderer versions with :</h3>

```bash
git submodule update --remote
```
<hr>

<h3>Libraries needed to compile this project : </h3>
<ul>
    <li>glfw 3</li>
    <li>glad 4.6 core</li>
    <li>freetype 2</li>
    <li>stb_image 2</li>
    <li>libmysql 6.1.11</li>
    <li>libcurl with https support</li>
</ul>

<hr>
<h3>Additionnal files to add for compiling : </h3>

NasaApodApiKey.h ( Where NASA_API_KEY has to be replaced with your NASA apod api key that you can get on : https://api.nasa.gov/ ) 
```C
#pragma once
#define NASA_APOD_API_KEY "NASA_API_KEY"
```

DatabaseLogin.h (where root and root have to be replace with your local mysql login)
```C
#pragma once
#define DATABASE_USER "root"
#define DATABASE_PASSWORD "root"
```
