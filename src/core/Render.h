/**
 * @file   Render.h
 * @author Odysseus Georgoudis
 * @date   01/09/2016
 * @brief  
 *
 */

#ifndef RAYTRACING_RENDER_H
#define RAYTRACING_RENDER_H

#include <fstream>
#include "../math/cyMath.h"

// Render definition (image output from the ray tracer)
class Render{
private:
    Color24 *render;
    float* z;
    unsigned char *zbuffer;
    int width, height;
    int size;
    int rendered;

public:

    // empty constructor
    Render(){
        render = NULL;
        z = NULL;
        zbuffer = NULL;
        width = 0;
        height = 0;
        size = 0;
        rendered = 0;
    }

    // initialization of render (need screen size)
    void init(int w, int h){
        width = w;
        height = h;
        size = w * h;
        if(render)
            delete[] render;
        render = new Color24[size];
        if(z)
            delete[] z;
        z = new float[size];
        for(int i = 0; i < size; i++)
            z[i] = FLOAT_MAX;
        if(zbuffer)
            delete[] zbuffer;
        zbuffer = NULL;
        reset();
    }

    // getters: width, height, size, render, buffer, rendered
    int getWidth() const
    {
        return width;
    }
    int getHeight() const
    {
        return height;
    }
    int getSize() const
    {
        return size;
    }

    Color24* getRenderImageArray() const
    {
        return render;
    }

    // FIXME breaking constness
    float* getZBufferArray() const
    {
        return z;
    }
    // FIXME breaking constness
    int getRendered() const
    {
        return rendered;
    }

    // reset the total number of rendered pixels
    void reset(){
        rendered = 0;
    }

    // increment total rendered pixels (one or arbitrary)
    void add(){
        rendered++;
    }
    void add(int n){
        rendered += n;
    }

    // check if render is done
    bool finished() const {
        return rendered >= size;
    }

    // calculate the z-buffer image
    void computeZBuffer(){

        // clear z-buffer image
        if(zbuffer)
            delete[] zbuffer;
        zbuffer = new unsigned char[size];

        // find min, max z-values
        float minZ = FLOAT_MAX;
        float maxZ = 0;
        for(int i = 0; i < size; i++){
            if(z[i] == FLOAT_MAX)
                continue;
            if(minZ > z[i])
                minZ = z[i];
            if(maxZ < z[i])
                maxZ = z[i];
        }

        // offset for background and object color
        int offset = 14;
        int contrast = -23;
        int mx = 255 + contrast - offset;

        // assign pixel values based on min & max z-values
        for(int i = 0; i < size; i++){

            // background color
            if(z[i] == FLOAT_MAX)
                zbuffer[i] = 0;

                // for pixels with objects, map from white (close) to dark (far)
            else{
                float f = (maxZ - z[i]) / (maxZ - minZ);
                int c = int(f * mx);
                if(c < 0)
                    f = 0;
                if(c > mx)
                    f = 2;
                zbuffer[i] = c + offset;
            }
        }
    }

    // save the rendered image to a file
    bool save(const char* file){
        return outputImage(file , 3);
    }

    // save the rendered z-buffer image to a file
    bool saveZBuffer(const char* file){
        return outputImage(file, 1);
    }

private:

    // write out an image file
    bool outputImage(const char* file, int components)
    {
        std::ofstream f;
        f.open(file);

        // if error writing to file
        if(!f)
            return false;

        // otherwise, output header in PPM format
        f << "P6\n" << width << " " << height << "\n255\n";

        // now write out image in binary
        for(int i = 0; i < size; i++){

            // output the rendered color image
            if(components == 3){
                unsigned char d[3] = {render[i].r, render[i].g, render[i].b};
                f.write(reinterpret_cast<char*>(d), sizeof(d));

                // output the z-buffer image
            }else if(components == 1){
                unsigned char d[3] = {zbuffer[i], zbuffer[i], zbuffer[i]};
                f.write(reinterpret_cast<char*>(d), sizeof(d));
            }
        }

        // close file
        f.close();
        return true;
    }
};

#endif //RAYTRACING_RENDER_H
