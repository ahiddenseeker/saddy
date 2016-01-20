#include "growingbinpacker.h"
#include "../../../include/3rdparty/framepacker/framepacker.hpp"

#include <QPainter>

// ============================  growingbinpacker::GrowingBinPacker::T methods ============================

growingbinpacker::GrowingBinPacker::T::T() :
m_width(0.0),
m_height(0.0),
m_t(NULL)
{
    
}

growingbinpacker::GrowingBinPacker::T::T(Texture* t) :
m_width(0.0),
m_height(0.0),
m_t(t)
{
    m_width = t->Image.width();
    m_height = t->Image.height();
}

double growingbinpacker::GrowingBinPacker::T::width() const
{
    return m_width;
}

double growingbinpacker::GrowingBinPacker::T::height() const
{
    return m_height;
}

double growingbinpacker::GrowingBinPacker::T::area() const
{
    return m_width * m_height;
}

void growingbinpacker::GrowingBinPacker::T::resize(double w, double h)
{
    m_width = w;
    m_height = h;
}

// ReSharper disable once CppMemberFunctionMayBeConst
int growingbinpacker::GrowingBinPacker::T::pixel(int, int )
{
    return static_cast<int>(area());
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppMemberFunctionMayBeStatic
void growingbinpacker::GrowingBinPacker::T::pixel(int x, int y, int p)
{
    
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppMemberFunctionMayBeStatic
bool growingbinpacker::GrowingBinPacker::T::is_transparent(int x, int y)
{
    return false;
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppMemberFunctionMayBeStatic
void growingbinpacker::GrowingBinPacker::T::copy_from(
        growingbinpacker::GrowingBinPacker::T block,
        int x, 
        int y,
        int width,
        int height,
        int px, 
        int py
)
{
    block.m_t->TextureRectangle = QRectF(QPointF(x + px,y + py), QSizeF(width, height));
}


// ============================  growingbinpacker::GrowingBinPacker methods ============================

growingbinpacker::GrowingBinPacker::GrowingBinPacker()
{
    
}

void growingbinpacker::GrowingBinPacker::pack(Atlas& atlas, QImage*& image)
{
    framepacker::packer<growingbinpacker::GrowingBinPacker::T> packer;
    packer.padding = 0;
    packer.alpha_trim = false;
    packer.allow_rotate = false;
    
    growingbinpacker::GrowingBinPacker::T* result = new growingbinpacker::GrowingBinPacker::T();
    framepacker::packer<growingbinpacker::GrowingBinPacker::T>::texture_type result_ptr(result);

    framepacker::packer<growingbinpacker::GrowingBinPacker::T>::texture_coll_type packed;
    framepacker::packer<growingbinpacker::GrowingBinPacker::T>::texture_coll_type failed;
    for(size_t i = 0; i < atlas.textures().size();  i++) {
        Texture* t = atlas.textures()[i];
        growingbinpacker::GrowingBinPacker::T* img = new growingbinpacker::GrowingBinPacker::T(t);

        packer.add(
            t->Name.toStdString(), 
            framepacker::packer<growingbinpacker::GrowingBinPacker::T>::texture_type(img)
        );
    }
    packer.pack(result_ptr, packed, failed);

    image = new QImage(result->width(), result->height(), QImage::Format_ARGB32);
    image->fill(QColor(255, 255, 255, 0));

    QPainter painter(image);
    for(size_t i = 0; i < atlas.textures().size(); i++)
    {
        painter.drawImage(
            atlas.textures()[i]->TextureRectangle.topLeft(),
            atlas.textures()[i]->Image
        );
    }
    painter.end();
}

growingbinpacker::GrowingBinPacker::~GrowingBinPacker()
{
    
}
