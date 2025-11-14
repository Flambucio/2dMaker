#include "RenderingSystem.h"
#include "CameraSystem.h"
#include "Animation.h"
using namespace D2Maker;

void RenderSystem::PushToQueue(EntityManager& em)
{
    for (auto entity : em.aliveEntities)
    {
        if (!em.hasComponent<TextureComponent>(entity)) { continue; }
        TextureComponent* texcomponent = em.getComponent<TextureComponent>(entity);
        if (!texcomponent->exists) { continue; }

        queue.push({ entity, texcomponent->orderInLayer });

        //TRACE("Pushed entity with texture: " +texcomponent->name);
    }
}

std::array<float, 16>  RenderSystem::GetVertices(Transform* t)
{
    //TRACE("Virtual Width: " + std::to_string(GameOptions::virtualWidth));
    float angle = t->rotationDegrees * (3.14159265f / 180.0f);
    float s = sin(angle);
    float c = cos(angle);

    float cx = t->x + t->width / 2.0f;
    float cy = t->y + t->height / 2.0f;

    std::array<std::pair<float, float>, 4> corners = {
        std::make_pair(t->x,            t->y),
        std::make_pair(t->x + t->width, t->y),
        std::make_pair(t->x + t->width, t->y + t->height),
        std::make_pair(t->x,            t->y + t->height)
    };

    std::array<float, 16> result;

    for (int i = 0; i < 4; i++)
    {
        float x = corners[i].first;
        float y = corners[i].second;
        float dx = x - cx;
        float dy = y - cy;
        float xRot = dx * c - dy * s + cx;
        float yRot = dx * s + dy * c + cy;
        float ndcX = ((2 * xRot) / GameOptions::virtualWidth) - 1;
        float ndcY = 1 - ((2 * yRot) / GameOptions::virtualHeight);


        result[i * 4 + 0] = ndcX;
        result[i * 4 + 1] = ndcY;
        result[i * 4 + 2] = (i == 1 || i == 2) ? 1.0f : 0.0f;
        result[i * 4 + 3] = (i >= 2) ? 1.0f : 0.0f;

    }

    return result;
}

void RenderSystem::RenderQueue(EntityManager& em)
{
    renderer.Clear();
    shaderProgram.Bind();
    while (!queue.empty())
    {
        Entity entity = queue.top().first;

        queue.pop();
        if (em.isVirtualEntity(entity)) { continue; }
        Transform* transformcomponent = em.getComponent<Transform>(entity);
        if (!ColliderFunctions::InViewport(transformcomponent)) { continue; }
        IndexBuffer ibo(indices, 6);

        TextureComponent* texcomponent = em.getComponent<TextureComponent>(entity);
        std::array<float, 16> vert = GetVertices(transformcomponent);
        VertexArray vao;
        vao.Bind();
        VertexBuffer vbo(vert.data(), 4 * 4 * sizeof(float));
        vao.AddBuffer(vbo, layout);

        TextureLoader::BindTexture(texcomponent->name);
        renderer.Draw(vao, ibo, shaderProgram);

    }
}

void CameraSystem::Update(EntityManager& em)
{
    if (em.cameraEntity == 0)
    {
        if (em.aliveEntities.find(0) != em.aliveEntities.end() && !em.hasComponent<Camera>(0))
        {
            return;
        }
    }
    if (em.isVirtualEntity(em.cameraEntity)) { return; }

    Transform* cameraT = em.getComponent<Transform>(em.cameraEntity);
    Camera* cameraC = em.getComponent<Camera>(em.cameraEntity);
    int dx = 0;
    int dy = 0;
    if (!cameraC) return;
    if (!cameraT) return;
    if (cameraC->enableX)
    {
        dx = cameraC->x - cameraT->x;
    }
    if (cameraC->enableY)
    {
        dy = cameraC->y - cameraT->y;
    }

    for (Entity entity : em.aliveEntities)
    {
        if (!em.hasComponent<Transform>(entity))
        {
            continue;
        }
        Transform* transform = em.getComponent<Transform>(entity);
        transform->x += dx;
        transform->y += dy;
    }




}



void AnimationSystem::IndexChanging(EntityManager& em)
{
    animEntities.clear();
    for (Entity entity : em.aliveEntities)
    {
        if (em.isVirtualEntity(entity)) { continue; }
        if (!em.hasComponent<Animation>(entity)) { continue; }
        animEntities.push_back(entity);
        Animation* animation = em.getComponent<Animation>(entity);
        animation->accumulator += DeltaTime::Get();
        if (animation->accumulator >= animation->timing)
        {
            TRACE(animation->texNames.size() - 1);
            if (animation->index < animation->texNames.size() - 1)
            {
                animation->index++;
            }
            else
            {
                animation->index = 0;
            }
            animation->accumulator -= animation->timing;
        }
    }
}
void AnimationSystem::TextureAdding(EntityManager& em)
{
    for (Entity entity : animEntities)
    {

        Animation* animation = em.getComponent < Animation>(entity);
        if (animation->index == animation->currentTextureIndex) { continue; }
        else { animation->currentTextureIndex = animation->index; }
        if (em.hasComponent<TextureComponent>(entity))
        {
            em.RemoveComponent<TextureComponent>(entity);
        }
        em.addComponent<TextureComponent>(entity, animation->texNames[animation->index],
            animation->orderInLayer);


    }
}