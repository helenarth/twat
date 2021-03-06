/*
 * Copyright (c) 2013 Christoph Malek
 * See LICENSE for more information.
 */

#ifndef CORE_TOOLS_TW_DATAFILES_MAP_H
#define CORE_TOOLS_TW_DATAFILES_MAP_H


#include <core/shared/graphics.h>

#include <core/tw_stripped/mapitems.h>

#include <string>
#include <map>
#include <vector>


namespace TWAT
{
	namespace TwTools
	{
		// extended map access


		class CTwMapInfo
		{
			std::string m_author;
			std::string m_version;
			std::string m_credits;
			std::string m_license;

		public:
			CTwMapInfo();
			void Clear();

			// setters
			void SetAuthor(const std::string &author);
			void SetVersion(const std::string &version);
			void SetCredits(const std::string &credits);
			void SetLicense(const std::string &license);

			// getters
			std::string Author() const {return m_author;}
			std::string Version() const {return m_version;}
			std::string Credits() const {return m_credits;}
			std::string License() const {return m_license;}
		};

		class CTwMapImage
		{
			std::string m_name;
			int m_height;
			int m_width;
			bool m_external;
			unsigned char *m_data;
			int m_dataSize;

		public:
			CTwMapImage();
			CTwMapImage(const CTwMapImage &other);
			~CTwMapImage();
			CTwMapImage &operator =(const CTwMapImage &other);

			// setters
			void SetName(const std::string &name);
			void SetSize(int height, int width);

			void LoadFromData(void *data, int size);
			bool LoadFromFile(const std::string &path);
			void Embed();
			void MakeExternal();


			// getters
			std::string Name() const {return m_name;}
			int Height() const {return m_height;}
			int Width() const {return m_width;}
			bool External() const {return m_external;}
			unsigned char *Data() const {return m_data;}
			int DataSize() const {return m_dataSize;}
		};

		class CTwMapLayer
		{
			int m_type;
			bool m_hasImage;
			std::string m_name;
			CTwMapImage m_image;

		public:
			CTwMapLayer();

			// setters
			void SetType(int type);
			void SetName(const std::string &name);
			void SetImage(const CTwMapImage &image);

			// getters
			int Type() const {return m_type;}
			bool HasImage() const {return m_hasImage;}
			std::string Name() const {return m_name;}
			CTwMapImage *Image() {return &m_image;}
		};

		class CTwMapLayerTiles : public CTwMapLayer
		{
			int m_width;
			int m_height;
			CTile *m_tiles;
			CColor m_color;

		public:
			CTwMapLayerTiles();
			CTwMapLayerTiles(const CTwMapLayerTiles &other);
			~CTwMapLayerTiles();

			// tiles interface
			void SetTiles(void *data);
			CTile *Tile(int index);

			// setters
			void SetSize(int height, int width);
			void SetColor(const CColor &color);

			// getters
			int Height() const {return m_height;}
			int Width() const {return m_width;}
			CColor *Color() {return &m_color;}
		};


		class CTwMapLayerQuads : public CTwMapLayer
		{
			int m_numQuads;

			std::vector<CQuad> m_quads;

		public:
			CTwMapLayerQuads();

			void AddQuad(const CQuad &quad);
			CQuad *Quad(int index);

			int NumQuads() const {return m_numQuads;}
		};

		class CTwMapGroup
		{
			std::string m_name;

			int m_offsetX;
			int m_offsetY;
			int m_paraX;
			int m_paraY;

			int m_numLayers;

			std::map<int, CTwMapLayer *> m_layers;

		public:
			CTwMapGroup();
			~CTwMapGroup();

			// setters
			void SetName(const std::string &name);
			void SetOffsetX(int offset);
			void SetOffsetY(int offset);
			void SetParaX(int para);
			void SetParaY(int para);

			// getters
			std::string Name() const {return m_name;}
			int OffsetX() const {return m_offsetX;}
			int OffsetY() const {return m_offsetY;}
			int ParaX() const {return m_paraX;}
			int ParaY() const {return m_paraY;}
			int NumLayers() const {return m_numLayers;}


			// layer inerface
			int NumTilesLayers();
			int NumQuadsLayers();

			void SetLayer(int index, CTwMapLayer *layer);
			template<typename LayerType> LayerType *Layer(int index)
			{
				if(index >= 0 && (m_layers.find(index) != m_layers.end()))
					return reinterpret_cast<LayerType *>(m_layers[index]); // return the requested layer addr

				return NULL;
			}
		};


		class CTwMap
		{
			// datafile access variables
			class CTwMapDataFileReader *m_reader;
			class CTwMapDataFileWriter *m_writer;

			// map variables
			CTwMapInfo m_mapInfo;
			std::vector<CTwMapGroup> m_groups;
			std::vector<CTwMapImage> m_images;

			bool m_validMap;
			std::string m_path;

			int m_numGroups;
			int m_groupsStart;
			int m_numLayers;
			int m_layersStart;
			int m_numImages;
			int m_imagesStart;


		public:			
			CTwMap();
			CTwMap(const std::string &path);
			~CTwMap();

			// loads a map from the given path
			//		returns:
			//				true:	on success
			//				false:	on failure
			bool Load(const std::string &path);

			// closes the map and writes down the changes
			void Close();


			// setters

			// getters
			bool IsValid() const {return m_validMap;}
			std::string Path() const {return m_path;}


			// interfaces
			// info
			void SetInfo(const std::string &author, const std::string &version, const std::string &credits, const std::string &license);
			CTwMapInfo Info() const {return m_mapInfo;}

			// groups
			void AddGroup(const CTwMapGroup &group);
			void SetGroup(int index, const CTwMapGroup &group);
			void DeleteGroup(int index);
			CTwMapGroup *Group(int index);
			int NumGroups() const {return m_numGroups;}

			// images (accessible also from a layer)
			CTwMapImage *Image(int index);
			int NumImages() const {return m_numImages;}


		private:
			// loads the info of the map (author, license...)
			// is called from this->Load
			void LoadInfo();

			// loads the images in an extra vector
			// to have quick-access to all images
			// is called from this->Load
			void LoadImages();

			// builds the main map structure (group->layer->image)
			// is called from this->Load
			void LoadStructure();
		};
	}
}

#endif // CORE_TOOLS_TW_DATAFILES_MAP_H
