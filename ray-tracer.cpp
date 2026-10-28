/// Include the image write library and enable it
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

/// Uncomment these lines if you want to debug using couts
#include <iostream>
#include <vector>
#include "Vector3.hpp"
#include "Triangle.hpp"
#include "file_util.h"
using namespace std;

int main()
{
  vector<vector<float>> trianglePoints = {};
  vector<vector<float>> triangleTextures = {};
  vector<vector<float>> triangleNormals = {};
  vector<vector<vector<int>>> triangleFaceNumbers = {};
  //util file example
  vector<string> lines = readLines("Untitled.obj");
  for(const string& line : lines){
    // cout << line << endl;
    vector<string> parts = split(line, ' ');
    // cout << parts[0] << endl;
    // cout << parts[1] << endl;
    // cout << parts[2] << endl;
    if(parts[0]=="v"){
      //cout << "Test1a" << endl;
      //vector<float> tempPoints = {stof(parts[1]), stof(parts[2]), stof(parts[3])};
      trianglePoints.push_back({stof(parts[1]), stof(parts[2]), stof(parts[3])});
      //cout << "Test2a" << endl;
    }
    else if(parts[0] == "vt"){
      //cout << "Test1b" << endl;
      triangleTextures.push_back({stof(parts[1]), stof(parts[2])});
      //cout << "Test2b" << endl;
    }
    else if(parts[0] == "vn"){
      //cout << "Test1c" << endl;
      triangleNormals.push_back({stof(parts[1]), stof(parts[2]), stof(parts[3])});
      //cout << "Test2c" << endl;
    }
    else if(parts[0] == "f"){
      // cout << "Test1d" << endl;
      vector<vector<int>> temp;
      for(const string& moreParts : parts){
        if(moreParts != "f"){
          vector<string> partsOfParts = split(moreParts, '/');
            temp.push_back({stoi(partsOfParts[0]), stoi(partsOfParts[1]), stoi(partsOfParts[2])});
        }
      }
      triangleFaceNumbers.push_back(temp);
      // cout << "Test2d" << endl;
    }
  }

  // cout << trianglePoints[1][0] << endl;
  // cout << trianglePoints[2][1] << endl;
  // cout << trianglePoints[0][2] << endl;
  // cout << triangleTextures[0][0] << endl;
  // cout << triangleTextures[0][1] << endl;
  // cout << triangleNormals[0][0] << endl;
  // cout << triangleNormals[0][1] << endl;
  // cout << triangleNormals[0][2] << endl;
  // cout << triangleFaceNumbers[0][0][2] << endl;
  // cout << triangleFaceNumbers[0][1][0] << endl;
  // cout << triangleFaceNumbers[0][2][0] << endl;
  // cout << triangleFaceNumbers[0][2][1] << endl;

  /// Example of how to debug with couts. 
  /// You must uncomment the include and namespace lines above
  //cout << "Hello, world" << endl;
  Vector3 temp = Vector3(0,0,0);
  temp += Vector3{1,1,1};

  //cout << temp << endl;

  /// Width of the final image
  const int w = 640;

  /// Height of the final image
  const int h = 640;

  /// The 1d array for storing the pixel we have generated
  unsigned char pixels[w * h * 3];

  // Triangle triangle = Triangle{Vector3{-1,1,0}, Vector3{-1,-1,0}, Vector3{1,-1,0}};
  // Triangle triangle2 = Triangle{Vector3{-1,1,0.5}, Vector3{-1,-1,-.5}, Vector3{1,-1,.5}};

  vector<Triangle> triangles = {};

  for(const vector<vector<int>> i : triangleFaceNumbers){
    //cout << i[0][0] << endl;
    triangles.push_back(Triangle{
      Vector3{
        trianglePoints[i[0][0] - 1][0],
        trianglePoints[i[0][0] - 1][1],
        trianglePoints[i[0][0] - 1][2],
      },
      Vector3{
        trianglePoints[i[1][0] - 1][0],
        trianglePoints[i[1][0] - 1][1],
        trianglePoints[i[1][0] - 1][2],
      },
      Vector3{
        trianglePoints[i[2][0] - 1][0],
        trianglePoints[i[2][0] - 1][1],
        trianglePoints[i[2][0] - 1][2],
      }
    });
  }
  //Ok hear me out, need triangle to be doing stuff but idk rn if triangleFaceNumbers should be a 2d or 3d vector
  //but you gotta have faith, glhf!!!

  Vector3 background_color = Vector3{150, 0, 255};

  /// Loop over each line doing down  
  for (int y = 0; y < h; y++)
  { 
    /// While going down, loop over every pixel going right
    for (int x = 0; x < w; x++)
    {

      int index = y * w * 3 + x * 3;
      
      /// The value of the red channel (0-255)
      unsigned char r = background_color.x;
      
      /// The value of the green channel (0-255)
      unsigned char g = background_color.y;
      
      /// The value of the blue channel (0-255)
      unsigned char b = background_color.z;
      
      //How do I ray trace?
      Vector3 camera_origin = Vector3{0, 0, 1};
      Vector3 camera_look_at = Vector3{0, 0, 0};
      Vector3 camera_up = Vector3{0, 1, 0};


      Vector3 target{
        x/(float)w * 2 - 1, -(y/(float)h * 2 - 1), 0
      };

      Vector3 temp = (target - camera_origin);

      Vector3 ray = temp.normalize();

      float closest_t = 1000000;

      for(const Triangle &triangle : triangles)
      {
        Vector3 edge_one = triangle.one - triangle.two;
        Vector3 edge_two = triangle.three - triangle.two;
        Vector3 p = edge_two.cross(edge_one);
        Vector3 abc = p.normalize();
        float d = -abc.dot(triangle.one);
        /// Figure out our index in the 1d array of pixels
        int index = y * w * 3 + x * 3;
        
        float t = -(abc.dot(camera_origin)+d)/abc.dot(ray);

        if(t > closest_t)
          continue;
    
        Vector3 collision_point = ray*t + camera_origin;
    
        Vector3 minus_one = collision_point - triangle.one;
        Vector3 minus_two = collision_point - triangle.two;
        Vector3 minus_three = collision_point - triangle.three;
    
        float triangle_area = p.length()/2;
    
        float area_one = minus_two.cross(minus_three).length()/2;
        float area_two = minus_three.cross(minus_one).length()/2;
        float area_three = minus_one.cross(minus_two).length()/2;
    
        float u = area_one/triangle_area;
        float v = area_two/triangle_area;
        float a = area_three/triangle_area;
    
        float sum = u + v + a;
    
        float diff = abs(1-sum);
    
        if(diff > 0.00001){
         //not in triangle
        }
        else{
          //in triangle

          closest_t = t;

          r = 0 * u + 0 * b + 255 * a;
          g = 0 * u + 255 * b + 0 * a;
          b = 255 * u + 0 * b + 0 * a;
        }
      }
      pixels[index] = r;
      pixels[index+1] = g;
      pixels[index+2] = b;
    }
  }

  ///Write the final image as a png
  stbi_write_png("image.png", w, h, 3, pixels, w * 3);
  return 0;
}
