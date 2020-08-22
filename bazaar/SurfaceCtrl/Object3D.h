#ifndef _SurfaceCtrl_Object3D_h_
#define _SurfaceCtrl_Object3D_h_
#include <Core/Core.h>
#include <GLCtrl_glad/GLCtrl_glad.h>
#include <Surface/Surface.h>
#include "Transform.h"
#include "Shader.h"
#include "BoundingBox.h"

#include "Mesh.h"

#include "STLLoader.h"
#include "OBJLoader.h"
namespace Upp{
enum DrawType { DT_TRIANGLE, DT_QUAD };
/*
struct Light{
	Light(){
		Position =vec3( 1.2, 80.0, 2.0);
		Ambient =vec3( 0.9, 0.9, 0.9);
		Diffuse =vec3( 0.5, 0.5, 0.5);
		Specular =vec3( 0.0, 1.0, 1.0);
	}
	Light(glm::vec3 _position, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular){
		Position = _position;
	    Ambient  = _ambient;
	    Diffuse  = _diffuse;
	    Specular = _specular;
	}
	
	glm::vec3 Position;
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
};*/





class Surface;

class Object3D : public Upp::Moveable<Object3D>{
	private:
		static int GlobalID;
		int ID;
		Vector<Mesh> meshes;
		bool loaded = false;
		bool moved = false;
		
		Material material; //The material object is a representation of material property of the object (it change how light affect it)
		
		Color lineColor = Black();
		float lineOpacity = 0.5f;
		float lineWidth = 1.0f;
		
		Color normalColor = Red();
		float normalOpacity = 0.5f;
		float normalLenght = 1.0f;
		
		OpenGLProgram NoLight; //The program will draw figure without light
		OpenGLProgram Line; //THe program will draw figure line
		OpenGLProgram Normal; //The program will draw Normal
		OpenGLProgram Light; //the program will draw figure with light

		bool showMesh = true;
		bool showMeshLine = false;
		bool showMeshNormal = false;
		bool showLight = true;
		
		BoundingBox boundingBox;
		bool showBoundingBox = false;
			
		Transform transform;
		
		GLenum DrawType = GL_TRIANGLES;
	
		bool UpdateBuffer(GLuint buffer, int SurfaceCount , int SurfaceNumber,int count , const float * data)noexcept;
		Vector<float> ReadBuffer(GLuint buffer, int SurfaceCount , int SurfaceNumber,int count)noexcept;
	public:
		Object3D():ID(GlobalID++){}
		//move will prevent your object to be deleted (from OpenGL perspective)
		Object3D(Object3D&& obj):ID(GlobalID++){*this = pick(obj);}
		Object3D& operator=(Object3D&& obj);
		
		//copy also VBO and VAO , wich mean when object is destroyed , every copy will
		//lost their 3D representation in OpenGL
		Object3D(const Object3D& obj):ID(GlobalID++){*this = obj;}
		Object3D& operator=(const Object3D& obj);
		~Object3D();

		bool LoadObj(const String& FileObj);
		bool LoadStl(const String& StlFile, Upp::Color = Green());
		bool LoadSurface(Surface& surface, Upp::Color = Green());
		Surface GetSurface();
		
		int GetID()const {return ID;}
		
		const Upp::Vector<Mesh>& GetMeshes() const noexcept{return meshes;}
		
		Upp::Vector<float>& GetVerticesData(int MeshNo = 0){if(MeshNo < meshes.GetCount()){return meshes[MeshNo].GetVertices();}else if(meshes.GetCount() > 0){return meshes[0].GetVertices();}else{throw Exc(Format(t_("Object3D '%i' don't have any meshes, vertices data can't be retrieve\n"), ID));}}
		Upp::Vector<float>& GetNormalsData(int MeshNo = 0){if(MeshNo < meshes.GetCount()){return meshes[MeshNo].GetNormals();}else if(meshes.GetCount() > 0){return meshes[0].GetNormals();}else{throw Exc(Format(t_("Object3D '%i' don't have any meshes, normals data can't be retrieve\n"), ID));}}
		Upp::Vector<float>& GetColorsData(int MeshNo = 0){if(MeshNo < meshes.GetCount()){return meshes[MeshNo].GetColors();}else if(meshes.GetCount() > 0){return meshes[0].GetColors();}else{throw Exc(Format(t_("Object3D '%i' don't have any meshes, colors data can't be retrieve\n"), ID));}}
		Upp::Vector<float>& GetTexturesData(int MeshNo = 0){if(MeshNo < meshes.GetCount()){return meshes[MeshNo].GetTexCoords();}else if(meshes.GetCount() > 0){return meshes[0].GetTexCoords();}else{throw Exc(Format(t_("Object3D '%i' don't have any meshes, textures coordinates data can't be retrieve\n"), ID));}}
		
		//Create a mesh if no mesh exist
		Object3D& AddVerticesData(const Upp::Vector<float>& data, int MeshNo = 0)noexcept{if(meshes.GetCount() == 0 && MeshNo == 0)meshes.Create();  if(MeshNo < meshes.GetCount()){meshes[MeshNo].GetVertices().Append(data);} return *this;}
		Object3D& AddNormalsData(const Upp::Vector<float>& data, int MeshNo = 0)noexcept{if(meshes.GetCount() == 0 && MeshNo == 0)meshes.Create(); if(MeshNo < meshes.GetCount()){meshes[MeshNo].GetNormals().Append(data);} return *this;}
		Object3D& AddColorsData(const Upp::Vector<float>& data, int MeshNo = 0)noexcept{if(meshes.GetCount() == 0 && MeshNo == 0)meshes.Create(); if(MeshNo < meshes.GetCount()){meshes[MeshNo].GetColors().Append(data);} return *this;}
		Object3D& AddTexturesData(const Upp::Vector<float>& data, int MeshNo = 0)noexcept{if(meshes.GetCount() == 0 && MeshNo == 0)meshes.Create(); if(MeshNo < meshes.GetCount()){meshes[MeshNo].GetTexCoords().Append(data);} return *this;}
		
		bool Load(); //Load all data in graphic memory It's called automaticly by using Load function, but you must call it if you set manually all data
		Object3D& Unload();
		
		Object3D& SetDrawType(GLenum dt)noexcept{DrawType = dt; return *this;}
		Object3D& ShowMesh(bool b = true)noexcept{showMesh = b; return *this;}
		Object3D& ShowMeshLine(bool b = true)noexcept{showMeshLine = b; return *this;}
		Object3D& ShowMeshNormal(bool b = true)noexcept{showMeshNormal = b; return *this;}
		Object3D& ShowLight(bool b = true)noexcept{showLight = b; return *this;}
		Object3D& ShowBoundingBox(bool b = true)noexcept{showBoundingBox = b; return *this;}
		
		bool GetShowMesh()const noexcept{return showMesh;}
		bool GetShowMeshLine()const noexcept{return showMeshLine;}
		bool GetShowMeshNormal()const noexcept{return showMeshNormal;}
		bool GetShowLight()const noexcept{return showLight;}
		bool GetShowBoundingBox()const noexcept{return showBoundingBox;}
		
		Object3D& SetLineColor(Color color)noexcept{lineColor = color; return *this;}
		Object3D& SetNormalColor(Color color)noexcept{normalColor = color; return *this;}
		Object3D& SetLineOpacity(float opacity)noexcept{lineOpacity = opacity; if(lineOpacity < 0) lineOpacity =0; if(lineOpacity> 1.0f) lineOpacity = 1.0f; return *this;}
		Object3D& SetLineWidth(float width)noexcept{lineWidth = width; if(lineWidth < 1) lineWidth = 1.0f; return *this;}
		Object3D& SetNormalOpacity(float opacity)noexcept{normalOpacity = opacity; if(normalOpacity < 0) normalOpacity =0; if(normalOpacity> 1.0f) normalOpacity = 1.0f; return *this;}
		Upp::Color GetLineColor()const noexcept{return lineColor;}
		Upp::Color GetNormalColor()const noexcept{return normalColor;}
		float GetNormalOpcaity()const noexcept{return normalOpacity;}
		float GetLineOpcaity()const noexcept{return lineOpacity;}
		float GetLineWidth()const noexcept{return lineWidth;}
		GLenum GetDrawType()const noexcept{return DrawType;}
		
		Material& GetMaterial(){return material;}
		
		Transform& GetTransform()noexcept{return transform;}
		const Transform& GetTransform()const noexcept{return transform;}
				
		void CreateBoundingBox(); // Create Bounding box
		void RemoveBoundingBox();
		
		BoundingBox& GetBoundingBox(){return boundingBox;}
		BoundingBox GetBoundingBoxTransformed()const noexcept{
			BoundingBox box(boundingBox);
			return box.TransformBy(transform.GetModelMatrix());
		}
		
		bool TestLineIntersection(const glm::vec3 & start, const glm::vec3 & end)const{
			BoundingBox box  = Upp::pick(GetBoundingBoxTransformed());
			return box.LineIntersection(start,end);
		}
		/*
			Write into OpenGL buffer return true if operation have been done correctly.
			/!\ True do not mean OpenGL will provide good things, it just mean buffer have been
			    writted  correctly
		*/
		bool UpdateColor(unsigned int MeshNo, unsigned int SurfaceNumber, int r, int g, int b)noexcept;
		bool UpdateColor(unsigned int MeshNo, unsigned int SurfaceNumber, float r, float g, float b)noexcept;
		bool UpdateColor(unsigned int MeshNo, unsigned int SurfaceNumber, glm::vec3 color)noexcept;
		bool UpdateColor(unsigned int MeshNo, unsigned int SurfaceNumber, Upp::Color color)noexcept;
		bool UpdateColors(unsigned int MeshNo, unsigned int SurfaceNumber,int Count, const float * data)noexcept;
		
		bool UpdateNormal(unsigned int MeshNo, unsigned int SurfaceNumber, float x, float y, float z)noexcept;
		bool UpdateNormal(unsigned int MeshNo, unsigned int SurfaceNumber, glm::vec3 normal)noexcept;
		bool UpdateNormals(unsigned int MeshNo, unsigned int SurfaceNumber,int Count, const float * data)noexcept;
		
		bool UpdateVertice(unsigned int MeshNo, unsigned int SurfaceNumber, float x, float y, float z)noexcept;
		bool UpdateVertice(unsigned int MeshNo, unsigned int SurfaceNumber, glm::vec3 vertice)noexcept;
		bool UpdateVertices(unsigned int MeshNo, unsigned int SurfaceNumber,int Count, const float * data)noexcept;
		/*
			Read OpenGL buffer and return data in Upp::Vector
		*/
		Vector<float> ReadColors(int MeshNo, unsigned int SurfaceNumber, int count);
		Vector<float> ReadNormals(int MeshNo, unsigned int SurfaceNumber, int count);
		Vector<float> ReadVertices(int MeshNo, unsigned int SurfaceNumber, int count);
		
		Object3D& SetProgramNoLight(const OpenGLProgram& program)noexcept{NoLight = program; return *this;}
		Object3D& SetProgramLight(const OpenGLProgram& program)noexcept{Light = program; return *this;}
		Object3D& SetProgramLine(const OpenGLProgram& program)noexcept{Line = program; return *this;}
		Object3D& SetProgramNormal(const OpenGLProgram& program)noexcept{Normal = program; return *this;}
		
		void Draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::vec3 viewPosition)noexcept;
};
	
}

#endif
