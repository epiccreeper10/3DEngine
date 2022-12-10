#include "spriteAnimComponent.h"
#include "Framework/actor.h"
#include "Renderer/texture.h"

namespace neu
{
    bool SpriteAnimComponent::Write(const rapidjson::Value& value) const
    {
        return false;
    }

    bool SpriteAnimComponent::Read(const rapidjson::Value& value)
    {
        if (value.HasMember("sequences") && value["sequences"].IsArray())
        {
            for (auto& sequenceValue : value["sequences"].GetArray())
            {
                Sequence sequence;

                READ_DATA(sequenceValue, sequence.name);
                READ_DATA(sequenceValue, sequence.fps);
                READ_DATA(sequenceValue, sequence.num_columns);
                READ_DATA(sequenceValue, sequence.num_rows);
                READ_DATA(sequenceValue, sequence.start_frame);
                READ_DATA(sequenceValue, sequence.end_frame);
                READ_DATA(sequenceValue, sequence.loop);

                std::string texture_name;
                READ_DATA(sequenceValue, texture_name);

                sequence.texture = g_resourceManager.Get<Texture>(texture_name, g_renderer);

                m_sequences[sequence.name] = sequence;
            }
        }

        std::string default_sequence;
        if (!READ_DATA(value, default_sequence))
        {
            default_sequence = m_sequences.begin()->first;
        }

        SetSequence(default_sequence);

        return true;
    }

    void SpriteAnimComponent::Update()
    {
        // update frame
        frameTimer += (float)g_time.deltaTime;
        if (frameTimer >= 1.0f / m_sequence->fps)
        {
            frameTimer = 0;
            frame++;
            if (frame > m_sequence->end_frame)
            {
                if (m_sequence->loop)
                {
                    frame = m_sequence->start_frame;
                }
            }
        }
    }

    void SpriteAnimComponent::Draw(Renderer& renderer)
    {
        renderer.Draw(m_sequence->texture, GetSource(), m_owner->GetTransform(), registration, flipHorizontal);
    }

    void SpriteAnimComponent::SetSequence(const std::string& name)
    {
        if (m_sequence && m_sequence->name == name) { return; }

        if (m_sequences.find(name) != m_sequences.end())
        {
            m_sequence = &m_sequences[name];

            frame = m_sequence->start_frame;
            frameTimer = 0;
        }
    }

    Rect& SpriteAnimComponent::GetSource()
    {
        Vector2 cellSize = m_sequence->texture->GetSize() / Vector2{ m_sequence->num_columns, m_sequence->num_rows };

        int row;
        int column;

        if (!m_sequence->loop)
        {
            if (frame >= m_sequence->end_frame)
            {
                column = (m_sequence->end_frame - 1) % m_sequence->num_columns;
                row = (m_sequence->end_frame - 1) / m_sequence->num_columns;
            }
            else
            {
                column = (frame - 1) % m_sequence->num_columns;
                row = (frame - 1) / m_sequence->num_columns;
            }
        }
        else
        {
            column = (frame - 1) % m_sequence->num_columns;
            row = (frame - 1) / m_sequence->num_columns;
        }


        source.x = (int)(column * cellSize.x);
        source.y = (int)(row * cellSize.y);
        source.w = (int)(cellSize.x);
        source.h = (int)(cellSize.y);

        return source;
    }
}
