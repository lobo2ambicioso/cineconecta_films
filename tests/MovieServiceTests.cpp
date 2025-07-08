#include <gtest/gtest.h>                            // Framework de testing de Google
#include <memory>                                   // Para std::shared_ptr
#include "../src/service/MovieService.h"            // Servicio a probar
#include "../src/service/TmdbApiClient.h"           // Cliente base de TMDb

// Mock simple que evita peticiones reales a TMDb y devuelve una respuesta fija
class MockTmdbApiClient : public TmdbApiClient {
public:
    MockTmdbApiClient() : TmdbApiClient("fake_key") {}

    // Simula una respuesta a la búsqueda por título
    std::optional<nlohmann::json> searchMovies(const std::string& query) override {
        return nlohmann::json::parse(R"({
            "results": [
                {
                    "id": 1,
                    "title": "Película de prueba",
                    "release_date": "2022-08-01",
                    "poster_path": "/poster.jpg"
                }
            ]
        })");
    }
};

// Test unitario que verifica si MovieService devuelve correctamente un resultado de búsqueda por título
TEST(MovieServiceTest, SearchMovieByTitle_ReturnsOneResult) {
    auto mockClient = std::make_shared<MockTmdbApiClient>();
    MovieService service(mockClient);

    auto results = service.searchMovies("prueba");

    ASSERT_EQ(results.size(), 1);                          // Verifica que haya un resultado
    EXPECT_EQ(results[0].title, "Película de prueba");     // Verifica el título
    EXPECT_EQ(results[0].year, "2022");                    // Verifica el año extraído de release_date
}

// Test unitario para verificar el resultado completo al obtener detalles de una película
TEST(MovieServiceTest, GetMovieDetails_ReturnsFullMovieInfo) {
    // Mock completo con detalles, créditos y configuración simulada
    class FullMockTmdbApiClient : public TmdbApiClient {
    public:
        FullMockTmdbApiClient() : TmdbApiClient("fake_key") {}

        // Simula respuesta del endpoint de detalles
        std::optional<nlohmann::json> getMovieDetails(long tmdbId) override {
            return nlohmann::json::parse(R"({
                "id": 123,
                "title": "Mockpelícula",
                "overview": "Descripción falsa",
                "vote_average": 7.8,
                "release_date": "2023-05-10",
                "poster_path": "/poster_fake.jpg",
                "genres": [
                    {"id": 1, "name": "Acción"},
                    {"id": 2, "name": "Comedia"}
                ]
            })");
        }

        // Simula respuesta del endpoint de créditos
        std::optional<nlohmann::json> getMovieCredits(long tmdbId) override {
            return nlohmann::json::parse(R"({
                "crew": [
                    {"job": "Director", "name": "Directora Falsa"},
                    {"job": "Writer", "name": "Escritor Ficticio"}
                ],
                "cast": [
                    {"name": "Actor Uno"},
                    {"name": "Actor Dos"},
                    {"name": "Actor Tres"},
                    {"name": "Actor Cuatro"},
                    {"name": "Actor Cinco"}
                ]
            })");
        }

        // Simula la construcción de la URL de imagen
        std::string buildFullImageUrl(const std::string& relativePath, const std::string& size = "w342") override {
            return "https://image.tmdb.org/t/p/" + size + relativePath;
        }

        // Simula que la configuración se cargó exitosamente
        bool loadConfiguration() override {
            return true;
        }
    };

    auto mockClient = std::make_shared<FullMockTmdbApiClient>();
    MovieService service(mockClient);

    auto result = service.getMovieDetails(123);
    ASSERT_TRUE(result.has_value());  // Verifica que se haya obtenido un resultado

    // Validaciones de cada campo
    EXPECT_EQ(result->title, "Mockpelícula");
    EXPECT_EQ(result->year, "2023");
    EXPECT_EQ(result->description, "Descripción falsa");
    EXPECT_EQ(result->director, "Directora Falsa");
    EXPECT_EQ(result->writers, "Escritor Ficticio");
    EXPECT_EQ(result->actors, "Actor Uno, Actor Dos, Actor Tres, Actor Cuatro, Actor Cinco");
    EXPECT_EQ(result->genres, "Acción, Comedia");
    EXPECT_EQ(result->imageUrl, "https://image.tmdb.org/t/p/w342/poster_fake.jpg");
    EXPECT_NEAR(result->generalRating, 7.8, 0.01);  // Tolerancia en comparación de float
}

// Test unitario que evalúa búsquedas por género y por año
TEST(MovieServiceTest, SearchMovieByGenreOrYear_ReturnsExpectedResults) {
    class GenreYearMockTmdbApiClient : public TmdbApiClient {
    public:
        GenreYearMockTmdbApiClient() : TmdbApiClient("fake_key") {}

        // Simula descubrimiento por género
        std::optional<nlohmann::json> discoverByGenre(const std::string& genreId) override {
            if (genreId == "28") {
                return nlohmann::json::parse(R"({
                    "results": [
                        {
                            "id": 101,
                            "title": "Película de Acción",
                            "release_date": "2023-06-01",
                            "poster_path": "/accion.jpg"
                        }
                    ]
                })");
            }
            return std::nullopt;
        }

        // Simula descubrimiento por año
        std::optional<nlohmann::json> discoverByYear(int year) override {
            if (year == 2024) {
                return nlohmann::json::parse(R"({
                    "results": [
                        {
                            "id": 202,
                            "title": "Película del 2024",
                            "release_date": "2024-01-15",
                            "poster_path": "/2024.jpg"
                        }
                    ]
                })");
            }
            return std::nullopt;
        }

        // Simula URL de imagen
        std::string buildFullImageUrl(const std::string& relativePath, const std::string& size = "w342") override {
            return "https://mock.image.tmdb.org/" + size + relativePath;
        }

        bool loadConfiguration() override {
            return true;
        }
    };

    auto mockClient = std::make_shared<GenreYearMockTmdbApiClient>();
    MovieService service(mockClient);

    // Test por género
    auto resultsByGenre = service.searchMovies("", "28");
    ASSERT_EQ(resultsByGenre.size(), 1);
    EXPECT_EQ(resultsByGenre[0].title, "Película de Acción");
    EXPECT_EQ(resultsByGenre[0].year, "2023");

    // Test por año
    auto resultsByYear = service.searchMovies("", "", 2024);
    ASSERT_EQ(resultsByYear.size(), 1);
    EXPECT_EQ(resultsByYear[0].title, "Película del 2024");
    EXPECT_EQ(resultsByYear[0].year, "2024");
}
