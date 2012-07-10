# This class performs a texture rect computation and mutates input entry objects setting their coordinates, according to gluing order.
load "imagegluingorder.rb"

class ImageArranger

    class ImageBucket
    
        attr_accessor :images # Array of images inside of bucket
        attr_accessor :size   # Size part of bucket
        
        def initialize()
        
        end
        
        # Initializes bucket with one image
        def initialize(image)
            @images = [image]
            image.textureRectangle = [0,0, image.size[0], image.size[1]]
            @size = image.size()
        end
        # Shifts all images by the point, without modifying size. Used by merged to arrange one bucket against another. Point is Array[x,y]
        def shift(point)
            images.each{ |image|  
                image.textureRectangle[0] = image.textureRectangle[0] + point[0]
                image.textureRectangle[1] = image.textureRectangle[1] + point[1]
            }
        end
        # Merges two buckets in specified order
        def self.merge(bucket1, bucket2, order)
            bucketsize1 = bucket1.size
            bucketsize2 = bucket2.size
            result = ImageBucket.new()
            if (order == GlueModer::HORIZONTAL)
                # Handle horizontal merge
            else
                # Handle vertical merge
            end
            return result
        end
    end

    def self.nextPOT(value)
        if (value == 1)
            return 2
        end
        
        value = value - 1
        [1,2,4,8,16].each { |x|                 
            value = (value >> x) | value 
        }
        value = (value >> 32) | value if value.class == Bignum
        value = value + 1
        return value
    end
    
    public
    # Arranges an images in specified order
    # Also mutates images, setting their texture rects
    # Second parameter is an array of GlueOrder objects, describing and order
    # Third parameter totalSize Array[width,height] parameters of resulting texture
    # Returns width and height of resulting texture as one integer (since it square POT texture it's all equal)
    def arrange(images, order, totalSize)
        if (images.length>1)
            
        elsif (images.length==1)
            images[0].textureRectangle = [0,0, images[0].size[0], images[0].size[1]]
        end
        return ImageArranger.nextPOT(totalSize.max)
    end
end