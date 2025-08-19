#include "Renderer.hpp"
#include <limits>
#include <iostream>

namespace graphics {
    template<typename T>
    Renderer<T>::Renderer(SDL_Renderer* renderer, int screenWidth, int screenHeight) :
        renderer(renderer), screenWidth(screenWidth), screenHeight(screenHeight) {
        
        labelFont = TTF_OpenFont("assets/fonts/Miracode.ttf", 16);
        if (!labelFont) {
            std::cerr << "Warning: gagal memuat font " << std::endl;
        }
    }

    template<typename T>
    Renderer<T>::~Renderer() {
        if (labelFont) {
            TTF_CloseFont(labelFont);
        }
    }

    template<typename T>
    void Renderer<T>::clearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_RenderClear(renderer);
    }

    template<typename T>
    void Renderer<T>::present() {
        SDL_RenderPresent(renderer);
    }

    template<typename T>
    math::Vector3<T> Renderer<T>::project(const math::Vector3<T>& worldPoint, const math::Matrix4<T>& mvpMatrix) const {
        math::Vector4<T> clipCoords(worldPoint.x, worldPoint.y, worldPoint.z, static_cast<T>(1));
        math::Vector4<T> projected = mvpMatrix * clipCoords;

        if (std::abs(projected.w()) < std::numeric_limits<T>::epsilon()) {
            return math::Vector3<T>(
                std::numeric_limits<T>::quiet_NaN(),
                std::numeric_limits<T>::quiet_NaN(),
                std::numeric_limits<T>::quiet_NaN()
            );
        }

        math::Vector3<T> ndcCoords(
            projected.x() / projected.w(), 
            projected.y() / projected.w(), 
            projected.z() / projected.w()
        );

        T screenX = (ndcCoords.x + static_cast<T>(1)) * static_cast<T>(0.5) * static_cast<T>(screenWidth);
        T screenY = (static_cast<T>(1) - ndcCoords.y) * static_cast<T>(0.5) * static_cast<T>(screenHeight);

        return math::Vector3<T>(screenX, screenY, ndcCoords.z);
    }

    template<typename T>
    bool Renderer<T>::isValidScreenPoint(const math::Vector3<T>& screenPoint) const {
        return !std::isnan(screenPoint.x) && !std::isnan(screenPoint.y) &&
               screenPoint.x >= 0 && screenPoint.x < screenWidth &&
               screenPoint.y >= 0 && screenPoint.y < screenHeight &&
               screenPoint.z >= -1 && screenPoint.z <= 1; 
    }

    template<typename T>
    int Renderer<T>::computeOutCode(const math::Vector3<T>& point) const {
        int code = 0;
        if (point.x < 0) code |= 1;        
        if (point.x >= screenWidth) code |= 2;   
        if (point.y < 0) code |= 4;        
        if (point.y >= screenHeight) code |= 8;  
        return code;
    }

    template<typename T>
    bool Renderer<T>::clipLine(math::Vector3<T>& p1, math::Vector3<T>& p2) const {
        if (std::isnan(p1.x) || std::isnan(p1.y) || std::isnan(p2.x) || std::isnan(p2.y)) {
            return false;
        }

        int outcode1 = computeOutCode(p1);
        int outcode2 = computeOutCode(p2);

        while (true) {
            if ((outcode1 | outcode2) == 0) {
                return true; 
            }
            if ((outcode1 & outcode2) != 0) {
                return false; 
            }

            int outcodeOut = outcode1 ? outcode1 : outcode2;
            T x, y;

            if (outcodeOut & 8) {
                x = p1.x + (p2.x - p1.x) * (screenHeight - 1 - p1.y) / (p2.y - p1.y);
                y = screenHeight - 1;
            } else if (outcodeOut & 4) {
                x = p1.x + (p2.x - p1.x) * (0 - p1.y) / (p2.y - p1.y);
                y = 0;
            } else if (outcodeOut & 2) {
                y = p1.y + (p2.y - p1.y) * (screenWidth - 1 - p1.x) / (p2.x - p1.x);
                x = screenWidth - 1;
            } else if (outcodeOut & 1) {
                y = p1.y + (p2.y - p1.y) * (0 - p1.x) / (p2.x - p1.x);
                x = 0;
            }

            if (outcodeOut == outcode1) {
                p1.x = x; p1.y = y;
                outcode1 = computeOutCode(p1);
            } else {
                p2.x = x; p2.y = y;
                outcode2 = computeOutCode(p2);
            }
        }
    }

    template<typename T>
    bool Renderer<T>::clipToNearPlane(const math::Vector3<T>& worldP1, const math::Vector3<T>& worldP2, 
                                    const math::Matrix4<T>& viewMatrix, const math::Matrix4<T>& modelMatrix,
                                    math::Vector3<T>& clippedP1, math::Vector3<T>& clippedP2) const {
        math::Matrix4<T> modelViewMatrix = viewMatrix * modelMatrix;
        
        math::Vector4<T> viewP1 = modelViewMatrix * math::Vector4<T>(worldP1.x, worldP1.y, worldP1.z, 1.0f);
        math::Vector4<T> viewP2 = modelViewMatrix * math::Vector4<T>(worldP2.x, worldP2.y, worldP2.z, 1.0f);
        
        T nearPlane = static_cast<T>(-0.1);
        
        bool p1Behind = viewP1.z() > nearPlane; 
        bool p2Behind = viewP2.z() > nearPlane;
        
        if (p1Behind && p2Behind) {
            return false; 
        }
        
        if (!p1Behind && !p2Behind) {
            clippedP1 = worldP1;
            clippedP2 = worldP2;
            return true;
        }
        
        T t = (nearPlane - viewP1.z()) / (viewP2.z() - viewP1.z());
        
        math::Vector3<T> intersection(
            worldP1.x + t * (worldP2.x - worldP1.x),
            worldP1.y + t * (worldP2.y - worldP1.y), 
            worldP1.z + t * (worldP2.z - worldP1.z)
        );
        
        if (p1Behind) {
            clippedP1 = intersection;
            clippedP2 = worldP2;
        } else {
            clippedP1 = worldP1;
            clippedP2 = intersection;
        }
        return true;
    }

    template<typename T>
    void Renderer<T>::drawLine(const math::Vector3<T>& p1, const math::Vector3<T>& p2, const math::Matrix4<T>& mvpMatrix, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        math::Matrix4<T> identityModel = math::Matrix4<T>::identity();

        math::Vector3<T> clippedWorldP1 = p1;
        math::Vector3<T> clippedWorldP2 = p2;
        
        math::Vector3<T> sp1 = project(clippedWorldP1, mvpMatrix);
        math::Vector3<T> sp2 = project(clippedWorldP2, mvpMatrix);

        if (clipLine(sp1, sp2)) {
            SDL_SetRenderDrawColor(renderer, r, g, b, a);
            SDL_RenderDrawLine(renderer, 
                static_cast<int>(sp1.x), static_cast<int>(sp1.y), 
                static_cast<int>(sp2.x), static_cast<int>(sp2.y));
        }
    }

    template<typename T>
    void Renderer<T>::drawMesh(const graphics::Mesh<T>& mesh, const math::Matrix4<T>& modelMatrix, const math::Matrix4<T>& viewMatrix, const math::Matrix4<T>& projectionMatrix, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        math::Matrix4<T> mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
        std::vector<SDL_Point> points;
        points.reserve(mesh.faces.size() * 6);
        SDL_SetRenderDrawColor(renderer, r, g, b, a);

        for (const auto& face : mesh.faces) {
            if (face.size() < 3) continue;
            
            for (size_t i = 0; i < face.size(); ++i) {
                int idx1 = face[i];
                int idx2 = face[(i + 1) % face.size()];

                if (idx1 < 0 || idx1 >= static_cast<int>(mesh.vertices.size()) ||
                    idx2 < 0 || idx2 >= static_cast<int>(mesh.vertices.size())) {
                    std::cerr << "Index vertex invalid di face: " << idx1 << " atau " << idx2 << std::endl;
                    continue;
                }

                math::Vector3<T> p1 = mesh.vertices[idx1];
                math::Vector3<T> p2 = mesh.vertices[idx2];
                
                
                math::Vector3<T> clippedWorldP1, clippedWorldP2;
                if (!clipToNearPlane(p1, p2, viewMatrix, modelMatrix, clippedWorldP1, clippedWorldP2)) {
                    continue;
                }
                
                math::Vector3<T> sp1 = project(clippedWorldP1, mvpMatrix);
                math::Vector3<T> sp2 = project(clippedWorldP2, mvpMatrix);

                
                if (clipLine(sp1, sp2)) {
                    SDL_RenderDrawLine(renderer, 
                        static_cast<int>(sp1.x), static_cast<int>(sp1.y),
                        static_cast<int>(sp2.x), static_cast<int>(sp2.y));
                }

                points.push_back({static_cast<int>(sp1.x), static_cast<int>(sp1.y)});
                points.push_back({static_cast<int>(sp2.x), static_cast<int>(sp2.y)});
            }
        }
    SDL_RenderDrawLines(renderer, points.data(), points.size());
    }

    template<typename T>
    void Renderer<T>::drawAxes(const math::Matrix4<T>& viewProjectionMatrix) {
        
        drawLine(math::Vector3<T>(0, 0, 0), math::Vector3<T>(200, 0, 0), viewProjectionMatrix, 255, 0, 0, 255);
        
        drawLine(math::Vector3<T>(0, 0, 0), math::Vector3<T>(0, 200, 0), viewProjectionMatrix, 0, 255, 0, 255);
        
        drawLine(math::Vector3<T>(0, 0, 0), math::Vector3<T>(0, 0, 200), viewProjectionMatrix, 0, 0, 255, 255);
    }
    
    template<typename T>
    void Renderer<T>::drawAxesWithLabels(const math::Matrix4<T>& viewProjectionMatrix) {
        T axisLength = static_cast<T>(2.0);
        
        drawArrow(math::Vector3<T>(0, 0, 0), math::Vector3<T>(axisLength, 0, 0), 
                viewProjectionMatrix, 255, 0, 0, 255);
        drawText3D("X", math::Vector3<T>(axisLength + 0.2f, 0, 0), 
                viewProjectionMatrix, 255, 100, 100, 255);
        
        drawArrow(math::Vector3<T>(0, 0, 0), math::Vector3<T>(0, axisLength, 0), 
                viewProjectionMatrix, 0, 255, 0, 255);
        drawText3D("Y", math::Vector3<T>(0, axisLength + 0.2f, 0), 
                viewProjectionMatrix, 100, 255, 100, 255);
        
        drawArrow(math::Vector3<T>(0, 0, 0), math::Vector3<T>(0, 0, axisLength), 
                viewProjectionMatrix, 0, 0, 255, 255);
        drawText3D("Z", math::Vector3<T>(0, 0, axisLength + 0.2f), 
                viewProjectionMatrix, 100, 100, 255, 255);
    }

    template<typename T>
    void Renderer<T>::drawArrow(const math::Vector3<T>& start, const math::Vector3<T>& end, 
                            const math::Matrix4<T>& mvpMatrix, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        
        drawLine(start, end, mvpMatrix, r, g, b, a);
        
        
        math::Vector3<T> direction = (end - start);
        T length = std::sqrt(direction.x*direction.x + direction.y*direction.y + direction.z*direction.z);
        if (length > 0.001f) {
            direction.x /= length; direction.y /= length; direction.z /= length;
            
            T arrowSize = length * static_cast<T>(0.1); 
            
            
            math::Vector3<T> perpendicular1, perpendicular2;
            if (std::abs(direction.x) < 0.9f) {
                perpendicular1 = math::Vector3<T>(0, direction.z, -direction.y);
            } else {
                perpendicular1 = math::Vector3<T>(-direction.z, 0, direction.x);
            }
            perpendicular2 = direction.cross(perpendicular1);
            
            
            T len1 = std::sqrt(perpendicular1.x*perpendicular1.x + perpendicular1.y*perpendicular1.y + perpendicular1.z*perpendicular1.z);
            T len2 = std::sqrt(perpendicular2.x*perpendicular2.x + perpendicular2.y*perpendicular2.y + perpendicular2.z*perpendicular2.z);
            if (len1 > 0.001f) { perpendicular1.x /= len1; perpendicular1.y /= len1; perpendicular1.z /= len1; }
            if (len2 > 0.001f) { perpendicular2.x /= len2; perpendicular2.y /= len2; perpendicular2.z /= len2; }
            
            
            math::Vector3<T> headBase = end - direction * arrowSize;
            math::Vector3<T> head1 = headBase + perpendicular1 * arrowSize * static_cast<T>(0.5);
            math::Vector3<T> head2 = headBase - perpendicular1 * arrowSize * static_cast<T>(0.5);
            math::Vector3<T> head3 = headBase + perpendicular2 * arrowSize * static_cast<T>(0.5);
            math::Vector3<T> head4 = headBase - perpendicular2 * arrowSize * static_cast<T>(0.5);
            
            
            drawLine(end, head1, mvpMatrix, r, g, b, a);
            drawLine(end, head2, mvpMatrix, r, g, b, a);
            drawLine(end, head3, mvpMatrix, r, g, b, a);
            drawLine(end, head4, mvpMatrix, r, g, b, a);
        }
    }

    
    template<typename T>
    void Renderer<T>::drawText3D(const std::string& text, const math::Vector3<T>& worldPos, 
                                const math::Matrix4<T>& mvpMatrix, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        if (!labelFont) return;
        
        math::Vector3<T> screenPos = project(worldPos, mvpMatrix);
        if (isValidScreenPoint(screenPos)) {
            SDL_Color color = {r, g, b, a};
            SDL_Surface* surface = TTF_RenderText_Solid(labelFont, text.c_str(), color);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                if (texture) {
                    SDL_Rect destRect = {
                        static_cast<int>(screenPos.x), 
                        static_cast<int>(screenPos.y), 
                        surface->w, 
                        surface->h
                    };
                    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
                    SDL_DestroyTexture(texture);
                }
                SDL_FreeSurface(surface);
            }
        }
    }
    template class Renderer<float>;
    template class Renderer<double>;

} 