#pragma once

namespace Renderent {

	enum class ShaderDataType {

		None = 0,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool

	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Float:
			return 4;
		case ShaderDataType::Float2:
			return 4 * 2;
		case ShaderDataType::Float3:
			return 4 * 3;
		case ShaderDataType::Float4:
			return 4 * 4;
		case ShaderDataType::Mat3:
			return 4 * 3 * 3;
		case ShaderDataType::Mat4:
			return 4 * 4 * 4;
		case ShaderDataType::Int:
			return 4;
		case ShaderDataType::Int2:
			return 4 * 2;
		case ShaderDataType::Int3:
			return 4 * 3;
		case ShaderDataType::Int4:
			return 4 * 4;
		case ShaderDataType::Bool:
			return 1;
		}

		RE_CORE_ASSERT(false, "Unknown ShaderDataType!");
	}

	struct BufferElement {

		std::string Name;
		uint32_t Offset;
		uint32_t Size;
		ShaderDataType Type;
		bool Normalized;

		BufferElement() {}

		BufferElement(ShaderDataType type, const std::string& name)
		:Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(false) {
				
		}
		BufferElement(ShaderDataType type, const std::string& name, bool normalized)
			:Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0),
			Normalized(normalized)
		{

		}

		uint32_t GetElementCount() const{
			switch (Type) {
			case ShaderDataType::Float:
			case ShaderDataType::Int:
			case ShaderDataType::Bool:
				return 1;
			case ShaderDataType::Float2:
			case ShaderDataType::Int2:
				return 2;
			case ShaderDataType::Float3:
			case ShaderDataType::Int3:
				return 3;
			case ShaderDataType::Float4:
			case ShaderDataType::Int4:
				return 4;
			case ShaderDataType::Mat3:
				return 3 * 3;
			case ShaderDataType::Mat4:
				return 4 * 4;
			}
		}
	};

	class BufferLayout {
	public:

		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements) {
		
			CalculateOffsetsAndStride();
		}

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		inline uint32_t GetStride() const { return m_Stride; }

	private:
		void CalculateOffsetsAndStride() {
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements) {
				element.Offset = offset;
				offset += element.Size;
			}
			m_Stride = offset;
		}

		
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;

	};

	class VertexBuffer {

	public:
		virtual ~VertexBuffer() {}

		static VertexBuffer* Create(size_t size, float* data);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

	};

	class IndexBuffer {

	public:
		virtual ~IndexBuffer() {}

		static IndexBuffer* Create(uint32_t count, uint32_t* data);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

	};

}